#include "libstandard.h"

void uniqRandum(int *res,int num,int limit){
	int i,j,tmp;
	for(i=0;i<num;){
		tmp=rand()%limit;
		for(j=0;j<i;j++)
			if(res[j]==tmp) break;
		if(j==i){
			res[i]=tmp;
			i++;
		}
	}
}
void setDammyVariable(double *new,int numPattern,int data){
	memset(new,0,sizeof(double)*numPattern);
	new[data]=1.0;
}
void makeNewData(double *new,double *old,int limit,int *stack,var_info *vars){
			int i,idx=0;
			if(vars[0].type==DTYPE_DISCRETE)
				setDammyVariable(&new[0],vars[0].numPattern,old[0]);
			else
				new[0]=old[0]; 
			idx+=vars[0].numPattern;
			for(i=0;i<limit;i++){
				if(vars[i].type==DTYPE_DISCRETE){
					setDammyVariable(&new[idx],vars[stack[i]+1].numPattern,old[stack[i]+1]);
					idx+=vars[i].numPattern;
				}else{
					new[idx]=old[stack[i]+1]; 
					idx++;
				}
			}
}
static void __subBestModel(double *sample,int numSample,int dimention,var_info *vars,int *stack,int limit,int rank,int start,double *max,DPGMM **bestCtx){
	DPGMM *model;
	int i,j;
	double *newData,likely;
	int testCase[100];
	if(limit==rank){
		model=dpgmm_init(limit,6);
		newData=malloc(sizeof(double)*(limit+1));
		uniqRandum(testCase,100,numSample);
		for(i=0;i<numSample;i++){
			for(j=0;j<sizeof(testCase)/sizeof(testCase[0]);j++)
				if(testCase[j]==i) break;
			if(j<sizeof(testCase)/sizeof(testCase[0])) continue;
			makeNewData(newData,&sample[i*dimention],limit,stack,vars);
			dpgmm_add(model,newData);
		}
		free(newData);
		dpgmm_setDefaultsPrior(model);
		dpgmm_solv(model,10);	
		for(i=0;i<100;i++){
			makeNewData(newData,&sample[testCase[i]*dimention],limit,stack,vars);
			likely+=log(dpgmm_prob(model,newData));
		}
		if(*max < likely || bestCtx==NULL){
			*max=likely;
			if(*bestCtx) dpgmm_release(*bestCtx);
			*bestCtx=model;
		}else
			dpgmm_release(model);
	}
	for(i=start;i<dimention-1;i++){
		stack[rank]=i;
		__subBestModel(sample,numSample,dimention,vars,stack,limit,rank+1,i,max,bestCtx);
		stack[rank]=0;
	}
}
double bestModel(double *sample,int numSample,int dimention,var_info *vars,DPGMM **bestModel){
	int i,*stack;
	double max;
	max=-1.0;
	stack=calloc(1,sizeof(int)*(dimention-1));
	*bestModel=NULL;
	for(i=0;i<dimention-1;i++){
		__subBestModel(sample,numSample,dimention,vars,stack,i,0,0,&max,bestModel);
	}
	free(stack);
	return max;
}
