/**
 * @file mathematics.c
 * @brief mathematics relation utility
 * @author Y.Mitsui
 */
#include "libstandard.h"
#define EPSABS 1e-4
#define EPSREL 1e-4

/******************************************************************************/
/*!	@brief Grant unique random number whose value is less than specified value.

	@param[out]		res		num unique number
	@param[in]		num		number you want
	@param[in]		limit	maxium value
******************************************************************************/
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
typedef struct{
	int dimention;
	double *range;
	double (*func)(DPGMM *,double *);
	void *arg;
	double *stack;
	int rank;
}ingArg;
double __subMultiIntegrate(double x,void *arg){
	double result,error;
	size_t neval;
	ingArg *info=arg;
	gsl_function F;

	if(info->rank==info->dimention){
		info->stack[info->rank-1]=x;
		return info->func(info->arg,info->stack);
	}else{
		info->stack[info->rank-1]=x;
		info->rank++;
		F.function = &__subMultiIntegrate;
		F.params=info;
		gsl_integration_qng(&F,info->range[(info->rank-1)*info->dimention],info->range[(info->rank-1)*info->dimention+1],EPSABS,EPSREL,&result,&error,&neval);
		return result;
	}
}
double multiIntegrate(double *range,int dimention,double (*func)(DPGMM *,double *x),void *arg){
	double result,error;
	size_t neval;
	ingArg info;
	gsl_function F;

	info.dimention=dimention;
	info.rank=1;
	info.range=range;
	info.func=func;
	info.arg=arg;
	info.stack=malloc(sizeof(double)*dimention);
	
	F.function = &__subMultiIntegrate;
	F.params=&info;
	gsl_integration_qng(&F,range[0],range[1],EPSABS,EPSREL,&result,&error,&neval);
	return result;
}
