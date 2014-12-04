#ifndef LIBSTD_H
#define LIBSTD_H 1

#include <stdio.h>
#include <mysql/mysql.h>
#include <dpgmm.h>
#include <glib.h>
#include <apr_pools.h>
#include <apr_strings.h>

enum DB_VALUE_TYPE{
	D_TYPE_INT,
	D_TYPE_STRING
};
typedef struct{
	enum DB_VALUE_TYPE type;
	char *value;
}DB_VALUE;

/*for predict.c*/
enum DTYPE{
	DTYPE_SEAQUENCE,
	DTYPE_DISCRETE
};
typedef struct{
	enum DTYPE type;
	int numPattern;
}var_info;


MYSQL *std_db_open(char *host,char *db,char *user,char *password);
MYSQL_RES* std_db_read(MYSQL *conn,const char *sql);
my_ulonglong std_db_write(MYSQL *conn,const char *table,GHashTable *data);

#endif
