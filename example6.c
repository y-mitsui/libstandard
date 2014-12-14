#include "libstandard.h"
void *dpgmmTrain(double *sample,int numSample,int dimention){
	int i;

	DPGMM *ctx=dpgmm_init(dimention,6);
	for(i=0;i<numSample;i++){
		dpgmm_add(ctx,&sample[i*dimention]);
	}
	dpgmm_setDefaultsPrior(ctx);
	dpgmm_solv(ctx,10);
	return ctx;
}
double dpgmmPredict(void *arg,double *sample){
	double r= dpgmm_prob(arg,sample);
	if(r<1.0e-16){
		printf("erro prob:%lf\n",r);
	}
	return (r<1.0e-16) ? 1.0e-16 : r;
}
typedef struct {
	int dimention;
	gsl_vector *u;
	gsl_matrix *sigma;
}logNormal;
void *logNormalTrain(double *sample,int numSample,int dimention){
	int i,j,k;
	logNormal *r=malloc(sizeof(logNormal));
	r->dimention=dimention;
	r->u=gsl_vector_calloc(dimention);
	for(i=0;i<numSample;i++){
		for(j=0;j<dimention;j++){
			gsl_vector_set(r->u,j,gsl_vector_get(r->u,j)+log(sample[i*dimention+j]));
		}
	}
	gsl_vector_mul_constant(r->u,1.0/(double)numSample);
	r->sigma=gsl_matrix_calloc(dimention,dimention);
	/* variance */
	for(i=0;i<dimention;i++){
		for(j=0;j<numSample;j++){
			gsl_matrix_set(r->sigma,i,i,pow(gsl_vector_get(r->u,i)-log(sample[j*dimention+i]),2));
		}
	}
	for(i=0;i<dimention;i++){
		for(j=0;j<dimention;j++){
			if(i!=j){
				for(k=0;k<numSample;k+=dimention){
					double diffX=gsl_vector_get(r->u,i)-log(sample[k*dimention+i]);
					double diffY=gsl_vector_get(r->u,j)-log(sample[k*dimention+j]);
					gsl_matrix_set(r->sigma,i,j,gsl_matrix_get(r->sigma,i,j)+diffX*diffY/numSample);
				}
			}
		}
	}
	
	return r;
}
double logNormalPredict(void *arg,double *x){
	int i;
	logNormal *p=arg;
	gsl_vector *data=gsl_vector_alloc(p->dimention);
	for(i=0;i<p->dimention;i++){
		gsl_vector_set(data,i,x[i]);
	}
	double r=multi_log_normal_distribution(data,p->u,p->sigma);
	gsl_vector_free(data);
	return (r<1.0e-16) ? 1.0e-16 : r;
}
void logNormalFree(void *arg){
	logNormal *p=arg;
	gsl_vector_free(p->u);
	gsl_matrix_free(p->sigma);
	free(p);
}
typedef struct{
	gsl_vector *min;
	gsl_vector *max;
	int dimention;
}Uniform;
void *uniformTrain(double *sample,int numSample,int dimention){
	int i,j;
	Uniform *r=calloc(1,sizeof(Uniform));
	r->min=gsl_vector_calloc(dimention);
	r->max=gsl_vector_calloc(dimention);
	for(i=0;i<dimention;i++){
		gsl_vector_set(r->min,i,sample[i]);
		gsl_vector_set(r->max,i,sample[i]);
		for(j=1;j<numSample;j++){
			if(gsl_vector_get(r->min,i) > sample[j*dimention+i]){
				gsl_vector_set(r->min,i,sample[j*dimention+i]);
			}else if(gsl_vector_get(r->max,i) < sample[j*dimention+i]){
				gsl_vector_set(r->max,i,sample[j*dimention+i]);
			}
		}
	}
	return r;
}
double uniformPredict(void *arg,double *x){
	Uniform *ctx=arg;
	double r=uniformPdf(ctx->max,ctx->min);
	return (r<1.0e-16) ? 1.0e-16 : r;
}
void uniformFree(void *arg){
	Uniform *ctx=arg;
	gsl_vector_free(ctx->min);
	gsl_vector_free(ctx->max);
	free(ctx);
}
#define NUM_DATA_SAMPLE 500
int main(void){
	gsl_rng *rng = gsl_rng_alloc (gsl_rng_default);
	gsl_rng_set(rng,time(NULL));
	int i;
	void *(*trains[])(double *,int,int)={dpgmmTrain,logNormalTrain,uniformTrain};
	double (*predicts[])(void *,double *)={dpgmmPredict,logNormalPredict,uniformPredict};
	void (*frees[])(void *)={(void (*)(void *))dpgmm_release,logNormalFree,uniformFree};
	double sample[NUM_DATA_SAMPLE];
	srand48(time(NULL));
	for(i=0;i<NUM_DATA_SAMPLE;i++){
		sample[i]=/*exp(2.5*gsl_ran_gaussian(rng,1.0)+10)*/drand48()*2.0;
		if(sample[i]<1.0e-16)  sample[i]=1.0e-16;
	}
	int r=bestModel(sample,NUM_DATA_SAMPLE,1,trains,predicts,frees,sizeof(trains)/sizeof(trains[0]));
	printf("r:%d\n",r);
	return 0;
}
