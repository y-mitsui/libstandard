#include <stdio.h>
#include <math.h>
#include "libstandard.h"

typedef struct {
	gsl_vector *u;
	gsl_matrix *sigma;
	int dimention;
} arg1;
double func(void *arg,double *x){
	arg1 *p=arg;
	int i=0;
	gsl_vector *data=gsl_vector_alloc(p->dimention);
	for(i=0;i<p->dimention;i++){
		gsl_vector_set(data,i,x[i]);
	}
	double r=multi_log_normal_distribution(data,p->u,p->sigma);
	return r;
}
int main(void){
	arg1 arg;
	arg.u=gsl_vector_alloc(2);
	gsl_vector_set(arg.u,0,0);
	gsl_vector_set(arg.u,1,0);
	arg.sigma=gsl_matrix_alloc(2,2);
	gsl_matrix_set(arg.sigma,0,0,1.0);
	gsl_matrix_set(arg.sigma,0,1,0.5);
	gsl_matrix_set(arg.sigma,1,0,0.5);
	gsl_matrix_set(arg.sigma,1,1,1.0);
	double range[]={0.01,20,0.01,20};
	arg.dimention=2;
	printf("%lf\n",multiIntegrate(range,2,func,&arg));
	return 1;
}
