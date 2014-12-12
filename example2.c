#include <stdio.h>
#include <math.h>
#include "libstandard.h"

int main(void){
	double x,y;
	gsl_vector *data=gsl_vector_alloc(2);
	gsl_vector *u=gsl_vector_alloc(2);
	gsl_vector_set(u,0,0);
	gsl_vector_set(u,1,0);
	gsl_matrix *sigma=gsl_matrix_alloc(2,2);
	gsl_matrix_set(sigma,0,0,1.0);
	gsl_matrix_set(sigma,0,1,0.5);
	gsl_matrix_set(sigma,1,0,0.5);
	gsl_matrix_set(sigma,1,1,1.0);
	for(x=0.01;x<15.0;x+=0.2){
		for(y=0.01;y<15.0;y+=0.2){
			gsl_vector_set(data,0,x);
			gsl_vector_set(data,1,y);
			printf("%lf %lf %lf\n",x,y,multi_log_normal_distribution(data,u,sigma));
		}
	}
	return 1;
}
