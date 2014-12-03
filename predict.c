#include "libstandard.h"

double evaluete(dpgmm *ctx,double *sample,int numSample){
	double likely=0.0;
	for(i=0;i<numSample;i++){
		likely+=log(dpgmm_prob(ctx,&sample[i*dimention]));
	}
	return likely;
}
void bestModel(int numDesc,int limit,double *max,dpgmm **bestCtx){
	if(limit==rank){
		dpgmm_add(ctx,newData);
		tmp=evaluete(ctx);
		if(*max < tmp){
			*max=tmp;
			*bestCtx=ctx;
		}else
			dpgmm_free(ctx);
	}
	for(i=0;i<numDesc;i++){
		stack[rank]=i;
		bestModel(numDesc);
	}
}
int main(void){
	int dim=3;
	MYSQL *conn=std_db_open(NULL,NULL,"root","password");
	result=std_db_read(conn,"SELECT rank,pops,win_rate from race");
	while((row=mysql_result(result))){
		sample[i*dim]=atof(row[0]);
		sample[i*dim+1]=atof(row[1]);
		sample[i*dim+2]=atof(row[2]);
	}
	for(i=0;i<dim;i++)
		bestModel(dim,i,&max,&ctx);
	double test[]={1,1,0.8};
	dpgmm_prob(ctx,test)/dpgmm_prob(ctx2,&test[1]);
	return 0;
}
