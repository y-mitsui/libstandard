/**
 * @file db.c
 * @brief MySQL accessing functions.
 * @author Y.Mitsui
 */

#include "libstandard.h"

/******************************************************************************/
/*!	@brief Open MySQL

	@param[in]		host		Host name of Database server
	@param[in]		db			Database name
	@param[in]		user		user name
	@param[in]		password	password of user
	@return			Success:resultset of libmysql responced by mysql_store_result().
					Fail:NULL
******************************************************************************/
MYSQL *std_db_open(const char *host,const char *db,const char *user,const char *password){
	MYSQL *conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, host, user, password, db, 3306, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	return conn;
}
/******************************************************************************/
/*!	@brief Execute SQL required result

	@param[in]		conn	context responced by std_db_open
	@param[in]		sql		SQL sentence 
	@return			Success:resultset of libmysql responced by mysql_store_result().
					Fail:NULL
******************************************************************************/
MYSQL_RES* std_db_read(MYSQL *conn,const char *sql){
	if (mysql_query(conn,sql)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	return mysql_store_result(conn);
}
/******************************************************************************/
/*! @brief Make insert data required in std_db_write()

    @param[in]		type	set data type as string or integer
    @param[in]		data	write data
    @return         Success:Structure of dbData for inserting GHashTable's value.
					Fail: NULL
******************************************************************************/
dbData* std_db_data(enum DB_DATA_TYPE type,char *data){
	dbData *r=malloc(sizeof(dbData));
	if(r==NULL) return NULL;
	r->type=type;
	r->data=data;
	return r;
}
/******************************************************************************/
/*! @brief Insert data to table

	@param[in]		conn	context responced by std_db_open
    @param[in]		table	table name
    @param[in]		data	write data generated by std_db_data()
    @return         Success:primary key
******************************************************************************/
my_ulonglong std_db_write(MYSQL *conn,const char *table,GHashTable *data){
	GHashTableIter iter;
	apr_pool_t *pool=NULL;
	g_hash_table_iter_init(&iter,data);
	dbData *val;
	char *key,*keys,*vals;

	apr_pool_create(&pool,NULL);
	keys="";
	while(g_hash_table_iter_next(&iter,(gpointer*)(void*)&key,(gpointer*)(void*)&val)){
		keys=apr_psprintf(pool,"%s `%s`,",keys,key);
		if(val->type==D_TYPE_INT){
			vals=apr_psprintf(pool,"%s,%s,",vals,val->data);
		}else{
			vals=apr_psprintf(pool,"%s,'%s',",vals,val->data);
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
}
