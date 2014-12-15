#include "libstandard.h"

int main(void){
	int dim=3,i;
	//DPGMM *ctx;
	MYSQL *conn=std_db_open(NULL,"sample","root","password");
	MYSQL_RES *res=std_db_read(conn,"SELECT rank,pops,win_rate from race");
	int numSample = mysql_num_rows(res);
	//double max;
	var_info vars[3];
	double *sample=malloc(sizeof(double)*dim*numSample);
	char **row;

	bzero(vars,sizeof(vars));
	vars[0].type=DTYPE_DISCRETE;
	vars[0].numPattern=3;
	for(i=0;(row = mysql_fetch_row(res));i++){
		sample[i*dim]=atof(row[1]);
		sample[i*dim+1]=atof(row[2]);
		sample[i*dim+2]=atof(row[3]);
	}
	//max=bestFeaturesModel(sample,i,dim,vars,&ctx);
	//printf("max:%lf\n",max);
	return 0;
}
