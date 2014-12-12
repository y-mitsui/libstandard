#include "libstandard.h"


gsl_matrix *gsl_inverse(gsl_matrix *m){
	int s=0;
	gsl_permutation * p = gsl_permutation_alloc (m->size1);
	gsl_matrix *inv=gsl_matrix_clone(m);
	gsl_matrix *r=gsl_matrix_alloc(inv->size1,inv->size2);
	gsl_linalg_LU_decomp(inv,p,&s);
	gsl_linalg_LU_invert(inv,p,r);
	gsl_matrix_free(inv);
	gsl_permutation_free(p);
	return r;
}
double gsl_det(gsl_matrix *m){
	gsl_permutation * p = gsl_permutation_alloc (m->size1);
	gsl_matrix *lu=gsl_matrix_clone(m);
	int s=0;
	gsl_linalg_LU_decomp (lu,p,&s);           // LU分解
	double n = gsl_linalg_LU_det (lu,s);    // 行列式
	gsl_matrix_free(lu);
	gsl_permutation_free(p);
	
	return n;
}
