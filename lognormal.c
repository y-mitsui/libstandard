#include "libstandard.h"
double multi_log_normal_distribution(gsl_vector *data,gsl_vector *u,gsl_matrix *sigma){
	int i;
	
	/* 1/det(sigma) */
	double sigmaDetInv=pow(gsl_det(sigma),-0.5);
	
	/* 1/(y_1*y_2...y_p) */
	double dataMulInv=1.0;
	for(i=0;i<data->size;i++){
		dataMulInv*=gsl_vector_get(data,i);
	}
	dataMulInv=pow(dataMulInv,-1.0);

	double constant=pow(2*M_PI,-(double)data->size/2.0);
	
	/* log(y)-u */
	gsl_matrix *logDataSubUWidth=gsl_matrix_alloc(1,data->size);
	for(i=0;i<data->size;i++){
		gsl_matrix_set(logDataSubUWidth,0,i,log(gsl_vector_get(data,i))-gsl_vector_get(u,i));
	}
	/* (log(y)-u)*sigma^-1*/
	gsl_matrix *sigmaInv=gsl_inverse(sigma);
	gsl_matrix *dataSubUSigma=gsl_matrix_alloc(1,data->size);
	gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1.0,logDataSubUWidth, sigmaInv, 0.0, dataSubUSigma);

	/* mul log(y)-u*/
	gsl_matrix *solv1=gsl_matrix_alloc(1,1);
	gsl_blas_dgemm (CblasNoTrans, CblasTrans,1.0,dataSubUSigma, logDataSubUWidth, 0.0, solv1);
	
	double argExp=-1.0/2.0*gsl_matrix_get(solv1,0,0);

	return constant*sigmaDetInv*dataMulInv*exp(argExp);
}
double log_normal_distribution(double x,double u,double sigma){
	return 1.0/(sqrt(2*M_PI)*sigma*x)*exp(-pow(log(x)-u,2)/2*sigma*sigma);
}
