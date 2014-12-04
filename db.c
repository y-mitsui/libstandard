#include "libstandard.h"

MYSQL *std_db_open(char *host,char *db,char *user,char *password){
	MYSQL *conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, host, user, password, db, 3306, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	return conn;
}
MYSQL_RES* std_db_read(MYSQL *conn,const char *sql){
	if (mysql_query(conn,sql)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	return mysql_store_result(conn);
}
/*
my_ulonglong std_db_write(MYSQL *conn,const char *table,GHashTable *data){
	GHashTableIter iter;
	apr_pool_t *pool=NULL;
	g_hash_table_iter_init(&iter,data);
	int size=g_hash_table_size(data);
	mydb *val;
	char *key;

	apr_pool_create(&pool);
	while(g_hash_table_iter_next(&iter,(gpointer*)(void*)&key,(gpointer*)(void*)&val)){
		keys=apr_psprintf(pool,"%s `%s`,",keys,key);
		if(mydb->type==D_TYPE_INT){
			vals=apr_psprintf(pool,"%s,%s,",vals,val);
		}else{
			vals=apr_psprintf(pool,"%s,'%s',",vals,val);
		}
	}
	keys[strlen(keys)-1]='\0';
	vals[strlen(vals)-1]='\0';
	char *sql=apr_psprintf(pool,"INSERT INTO %s(%s) VALUES(%s)",table,keys,vals);
	if (mysql_query(conn,sql)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	apr_pool_destroy(pool);
	return mysql_insert_id(conn);
}*/
