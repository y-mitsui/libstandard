#include "libstandard.h"
void *dpgmmTrain(const double *sample,int numSample,int dimention){
	int i;

	DPGMM *ctx=dpgmm_init(dimention,6);
	for(i=0;i<numSample;i++){
		dpgmm_add(ctx,&sample[i*dimention]);
	}
	dpgmm_setDefaultsPrior(ctx);
	dpgmm_solv(ctx,10);
	return ctx;
}
double dpgmmPredict(void *arg,const double *sample){
	double r= dpgmm_prob(arg,sample);
	if(r<1.0e-16){
		printf("erro prob:%lf\n",r);
	}
	return (r<1.0e-16) ? 1.0e-16 : r;
}
void *logNormalTrain(const double *sample,int numSample,int dimention){
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
double logNormalPredict(void *arg,const double *x){
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
void *uniformTrain(const double *sample,int numSample,int dimention){
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
double uniformPredict(void *arg,const double *x){
	Uniform *ctx=arg;
	double r=uniformPdf(ctx->max,ctx->min,x);
	return (r<1.0e-16) ? 1.0e-16 : r;
}
void uniformFree(void *arg){
	Uniform *ctx=arg;
	gsl_vector_free(ctx->min);
	gsl_vector_free(ctx->max);
	free(ctx);
}

void bestPredictionFree(bestPrediction *ctx){
	ctx->free(ctx->ctx);
	free(ctx);
}
double bestPredictionProb(bestPrediction *ctx,const double *x){
	return ctx->predict(ctx->ctx,x);
}
bestPrediction *bestPredictionModel(const double *sample,int numSample,int dimention){
	bestPrediction *r;
	void *(*trains[])(const double *,int,int)={dpgmmTrain,logNormalTrain,uniformTrain};
	double (*predicts[])(void *,const double *)={dpgmmPredict,logNormalPredict,uniformPredict};
	void (*frees[])(void *)={(void (*)(void *))dpgmm_release,logNormalFree,uniformFree};
	double evaluation;
	int no=bestModel(sample,numSample,dimention,trains,predicts,frees,sizeof(trains)/sizeof(trains[0]),&evaluation);
	r=malloc(sizeof(bestPrediction));
	r->type=no;
	r->predict=predicts[no];
	r->free=frees[no];
	r->evaluation=evaluation;
	return r;
}
bestFeatures *trainFromDB(const char *sql,const char *host,const char *db,const char *user,const char *password,const var_info *vars){
	int numSample,dim;
	bestFeatures *model;
	double *sample=samplingFromDB(sql,host,db,user,password,&numSample,&dim);
	model=bestFeaturesModel(sample,numSample,dim,vars);
	return model;
}
