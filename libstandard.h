#ifndef LIBSTD_H
#define LIBSTD_H 1

#include <stdio.h>
#include <mysql/mysql.h>
#include <dpgmm.h>
#include <glib.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

enum DB_DATA_TYPE{
	D_TYPE_INT,
	D_TYPE_STRING
};
typedef struct{
	enum DB_DATA_TYPE type;
	char *data;
}dbData;

/*for predict.c*/
enum DTYPE{
	DTYPE_SEAQUENCE,
	DTYPE_DISCRETE
};
typedef struct{
	enum DTYPE type;
	int numPattern;
}var_info;
double bestModel(double *sample,int numSample,int dimention,var_info *vars,DPGMM **bestModel);


MYSQL *std_db_open(char *host,char *db,char *user,char *password);
MYSQL_RES* std_db_read(MYSQL *conn,const char *sql);
my_ulonglong std_db_write(MYSQL *conn,const char *table,GHashTable *data);

#endif
