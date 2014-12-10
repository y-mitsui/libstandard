#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

gsl_matrix *gsl_inverse(gsl_matrix *m){
	int s=0;
	gsl_linalg_LU_decomp(invShape,p,&s);
	ctx->shape=gsl_matrix_alloc(ctx->invShape->size1,ctx->invShape->size2);
	gsl_linalg_LU_invert(invShape,p,ctx->shape);
	return invShape
}
gsl_vector *m_log_normal_distribution(gsl_vector *data,gsl_vector *u,gsl_matrix *sigma){
	int i;
	gsl_matrix *tmp=gsl_matrix_alloc(sigma->size1,u->size);
	gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1.0, sigma, data,0.0, tmp);
	gsl_matrix_mul_const(tmp,sqrt(2*M_PI));
	for(i=0;i<data->size;i++){
		gsl_vector_set(data,i,1/2*pow(log(gsl_vector_get(data,i))-gsl_vector_get(u),2));
	}
	gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1.0,data, gsl_inverse(sigma), 0.0, tmp);
	for(i=0;i<tmp->size1;i++){
		for(j=0;j<tmp->size1;j++){
			gsl_vector_set(tmp,i,j,exp(gsl_matrix_get(tmp,i,j)));
		}
	}
	
}
double log_normal_distribution(double x,double u,double sigma){
	return 1.0/(sqrt(2*M_PI)*sigma*x)*exp(-pow(log(x)-u,2)/2*sigma*sigma);
}
int main(void){
	gsl_vector *data=gsl_vector_alloc(2);
	gsl_vecotr *u=gsl_vector_alloc(2);
	gsl_vector_set(u,0,0);
	gsl_vector_set(u,1,0);
	gsl_matrix *sigma=gsl_matrix_alloc(2,2);
	gsl_matrix_set(sigma,0,0,1);
	gsl_matrix_set(sigma,0,1,0);
	gsl_matrix_set(sigma,1,0,0);
	gsl_matrix_set(sigma,1,1,1);
	for(x=0.01;x<3.0;x+=0.01){
		for(y=0.01;y<3.0;y+=0.01){
			gsl_vector_set(data,0,x);
			gsl_vector_set(data,1,x);
			printf("%lf %lf %lf\n",x,y,m_log_normal_distribution(data,u,sigma));
		//printf("%lf %lf\n",x,log_normal_distribution(x,0.0,1.0));
		}
	}
}
