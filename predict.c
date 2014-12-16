/**
 * @file predict.c
 * @brief Data science utility
 * @author Y.Mitsui
 */

#include "libstandard.h"

static void setDammyVariable(double *new,int numPattern,int data){
	memset(new,0,sizeof(double)*numPattern);
	new[data]=1.0;
}
void makeDammyData(double *new,const double *old,int numSample,int dimention,int newDimention,const var_info *vars){
	int i,j,idx=0;
	for(i=0;i<numSample;i++){
		idx=0;
		for(j=0;j<dimention;j++){
			if(vars[j].type==DTYPE_DISCRETE){
				setDammyVariable(&new[i*newDimention+idx],vars[j].numPattern,(int)old[i*dimention+j]);
				idx+=vars[j].numPattern;
			}else{
				new[idx*newDimention]=old[j*dimention]; 
				idx++;
			}
		}
	}
}
int calcNewDimention(const var_info *vars,int limit){
	int r=0,i;
	r+=(vars[0].type==DTYPE_DISCRETE) ? vars[0].numPattern:1;
	for(i=0;i<limit;i++){
		r+=(vars[i].type==DTYPE_DISCRETE) ? vars[i].numPattern:1;
	}
	return r;
}
static double *newFeatures(const double *sample,int numSample,int dimention,int *stack,int numStack,var_info *new_vars,const var_info *vars){
	double *r=malloc(sizeof(double)*numSample*numStack);
	int i,j;

	for(i=0;i<numStack;i++){
		memcpy(&new_vars[i],&vars[stack[i]],sizeof(vars[i]));
	}
	for(i=0;i<numSample;i++){
		for(j=0;j<numStack;j++){
			r[i*numStack+j]=sample[i*dimention+stack[j]];
		}
	}
	return r;
}
#define NUM_TEST 10
static void __subBestModel(const double *sample,int numSample,int dimention,const var_info *vars,int *stack,int limit,int rank,int start,bestPrediction **bestCtx){
	//DPGMM *model;
	int i/*,j*/;
	double *newData/*,likely=0.0*/;
	//int testCase[NUM_TEST];
	if(limit==rank){
		var_info *new_vars=malloc(sizeof(var_info)*limit);
		double *newF=newFeatures(sample,numSample,dimention,stack,limit,new_vars,vars);
		int dim=calcNewDimention(new_vars,limit);
		newData=malloc(sizeof(double)*dim*numSample);
		makeDammyData(newData,newF,numSample,dimention,limit,new_vars);
		bestPrediction *ctx=bestPredictionModel(newData,numSample,dim);
		bestPrediction *tmp=*bestCtx;
		if(tmp->evaluation < ctx->evaluation){
			bestPredictionFree(tmp);
			*bestCtx=ctx;
		}else{
			bestPredictionFree(ctx);
		}
		free(newData);
		free(newF);
	}
	for(i=start;i<dimention-1;i++){
		stack[rank]=i;
		__subBestModel(sample,numSample,dimention,vars,stack,limit,rank+1,i,bestCtx);
		stack[rank]=0;
	}
}
/******************************************************************************/
/*!	@brief Grant best DPGMM model selected feature

	@param[in]		sample		sampling data. Format is sample[0],...,sample[dimention-1],...,sample[(numSample-1)*(dimention-1)].
	@param[in]		numSample	number of sample
	@param[in]		dimention	dimention of sample
	@param[in]		vars	infomation indicated discrete or sequence for each dimention
	@param[our]		bestModel	best DPGMM model computed
	@return			Success:cross validation likely of best model
					Fail:nagative
******************************************************************************/
double bestFeaturesModel(const double *sample,int numSample,int dimention,const var_info *vars,bestPrediction **bestModel){
	int i,*stack;
	stack=calloc(1,sizeof(int)*(dimention-1));
	*bestModel=NULL;
	for(i=0;i<dimention-1;i++){
		__subBestModel(sample,numSample,dimention,vars,stack,i,0,0,bestModel);
	}
	free(stack);
	return (*bestModel)->evaluation;
}

double crossValidationLikelihood(const double *sample,int numSample,int dimention,void *(*train)(const double *,int,int ),double (*predict)(void *,const double *),void (*modelFree)(void*)){
	int i=0;
	double *newSample;
	double likelihood=0.0;

	newSample=malloc(sizeof(double)*(numSample-1)*dimention);
	for(i=0;i<numSample;i++){
		memcpy(newSample,sample,i*dimention*sizeof(double));
		memcpy(newSample,&sample[(i+1)*dimention],(numSample-(i+1))*sizeof(double)*dimention);
		void *ctx=train(newSample,numSample-1,dimention);
		likelihood+=log(predict(ctx,&sample[i*dimention]));
		//printf("likelihood:%lf\n",likelihood);
		modelFree(ctx);
	}
	free(newSample);
	return likelihood;
}
double BIC(double likelihood,int num){
	return -likelihood+num;
}
enum EVA_TYPE{
	EVA_CROSS_VALIDATION,
	EVA_BIC
};
int bestModel(const double *sample,int numSample,int dimention,void *(**trains)(const double *,int,int),double (**predicts)(void *,const double *),void (**frees)(void*),int numModel,double *bestEva){
	int i;
	double max;
	int bestIdx=-1;
	double tmp;
	enum EVA_TYPE evaluationType=EVA_CROSS_VALIDATION;
	for(i=0;i<numModel;i++){
		switch (evaluationType){
		case EVA_CROSS_VALIDATION:
			tmp=crossValidationLikelihood(sample,numSample,dimention,trains[i],predicts[i],frees[i]);
			break;
		case EVA_BIC:
			tmp=1.0/BIC(0.0,0);
		}
		printf("tmp:%lf\n",tmp);
		if(bestIdx < 0 || max<tmp){
			max=tmp;
			bestIdx=i;
		}
	}
	*bestEva=max;
	return bestIdx;
}
