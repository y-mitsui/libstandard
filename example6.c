#include "libstandard.h"
#define NUM_DATA_SAMPLE 500
int main(void){
	gsl_rng *rng = gsl_rng_alloc (gsl_rng_default);
	gsl_rng_set(rng,time(NULL));
	int i;
	void *(*trains[])(const double *,int,int)={dpgmmTrain,logNormalTrain,uniformTrain};
	double (*predicts[])(void *,const double *)={dpgmmPredict,logNormalPredict,uniformPredict};
	void (*frees[])(void *)={(void (*)(void *))dpgmm_release,logNormalFree,uniformFree};
	double sample[NUM_DATA_SAMPLE];
	srand48(time(NULL));
	for(i=0;i<NUM_DATA_SAMPLE;i++){
		sample[i]=/*exp(2.5*gsl_ran_gaussian(rng,1.0)+10)*/drand48()*2.0;
		if(sample[i]<1.0e-16)  sample[i]=1.0e-16;
	}
	double tmp;
	int r=bestModel(sample,NUM_DATA_SAMPLE,1,trains,predicts,frees,sizeof(trains)/sizeof(trains[0]),&tmp);
	printf("r:%d\n",r);
	return 0;
}
