#include <stdio.h>
#include <math.h>
#include "libstandard.h"

int main(void){
	double x;
	gsl_vector *data=gsl_vector_alloc(1);
	gsl_vector *u=gsl_vector_alloc(1);
	gsl_vector_set(u,0,0);
	gsl_matrix *sigma=gsl_matrix_alloc(1,1);
	gsl_matrix_set(sigma,0,0,1.0);
	for(x=0.01;x<15.0;x+=0.2){
		gsl_vector_set(data,0,x);
		printf("%lf %lf %lf\n",x,multi_log_normal_distribution(data,u,sigma),log_normal_distribution(x,gsl_vector_get(u,0),gsl_matrix_get(sigma,0,0)));
	}
	return 1;
}
