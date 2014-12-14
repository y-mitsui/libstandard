#include <stdio.h>
#include <math.h>
#include "libstandard.h"

typedef struct {
	gsl_vector *alpha;
	gsl_vector *beta;
	int dimention;
} arg2;
double func(void *arg,double *x){
	arg2 *p=arg;
	int i=0;
	gsl_vector *data=gsl_vector_alloc(p->dimention);
	for(i=0;i<p->dimention;i++){
		gsl_vector_set(data,i,x[i]);
	}
	double r=uniformPdf(p->alpha,p->beta);
	printf("%lf\n",r);
	return r;
}
int main(void){
	printf("%lf\n",pow(4,0.5));
	exit(0);
	arg2 arg;
	arg.alpha=gsl_vector_alloc(2);
	gsl_vector_set(arg.alpha,0,0);
	gsl_vector_set(arg.alpha,0,0);
	arg.beta=gsl_vector_alloc(2);
	gsl_vector_set(arg.beta,0,6.0);
	gsl_vector_set(arg.beta,1,7.0);
	double range[]={0.0,6.0,0.0,7.0};
	arg.dimention=2;
	printf("%lf\n",multiIntegrate(range,2,func,&arg));
	return 1;
}
