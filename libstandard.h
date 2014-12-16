#ifdef _cplusplus
extern "C" {
#endif
#ifndef LIBSTD_H
#define LIBSTD_H 1

#include <stdio.h>
#include <mysql/mysql.h>
#include <dpgmm.h>
#include <glib.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <ctype.h>

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

enum ModelType{
	DPGMM_STICK_BREAK,
	LOG_NORMAL_LIKELYHOOD,
	UNIFORM_LIKELYHOOD
};
typedef struct{
	enum ModelType type;
	double (*predict)(void *,const double *);
	void (*free)(void*);
	void *ctx;
	double evaluation;
}bestPrediction;

typedef struct {
	int dimention;
	gsl_vector *u;
	gsl_matrix *sigma;
}logNormal;

typedef struct{
	gsl_vector *min;
	gsl_vector *max;
	int dimention;
}Uniform;

double bestFeaturesModel(const double *sample,int numSample,int dimention,const var_info *vars,bestPrediction **bestModel);
double crossValidationLikelihood(const double *sample,int numSample,int dimention,void *(*train)(const double *,int,int ),double (*predict)(void *,const double *),void (*modelFree)(void*));

xmlNodePtr xmlNodeGetChild(xmlNodePtr parent,int no);
char* xmlNodeGetContentP(apr_pool_t *pool,xmlNodePtr node);

MYSQL *std_db_open(const char *host,const char *db,const char *user,const char *password);
MYSQL_RES* std_db_read(MYSQL *conn,const char *sql);
my_ulonglong std_db_write(MYSQL *conn,const char *table,GHashTable *data);
dbData* std_db_data(enum DB_DATA_TYPE type,char *data);
GSList* query_xpath(htmlDocPtr doc,const char *query);
char *xpathOneContent(apr_pool_t *pool,htmlDocPtr doc,const char *xpath);
double *samplingFromDB(const char *sql,const char *host,const char *db,const char *user,const char *password,int *numSample,int *dim);

GSList* getFileList(const char *dir);

char *trim(char *str);

void uniqRandum(int *res,int num,int limit);
double multiIntegrate(double *range,int dimention,double (*func)(void *,const double *x),void *arg);
double log_normal_distribution(double x,double u,double sigma);


gsl_matrix *gsl_inverse(gsl_matrix *m);
double gsl_det(gsl_matrix *m);

double multi_log_normal_distribution(gsl_vector *data,gsl_vector *u,gsl_matrix *sigma);
double log_normal_distribution(double x,double u,double sigma);
double uniformPdf(gsl_vector *alpha,gsl_vector *beta,const double *x);

int bestModel(const double *sample,int numSample,int dimention,void *(**trains)(const double *,int,int),double (**predicts)(void *,const double *),void (**frees)(void*),int numModel,double *bestEva);

void *dpgmmTrain(const double *sample,int numSample,int dimention);
double dpgmmPredict(void *arg,const double *sample);
void *logNormalTrain(const double *sample,int numSample,int dimention);
double logNormalPredict(void *arg,const double *x);
void logNormalFree(void *arg);
void *uniformTrain(const double *sample,int numSample,int dimention);
double uniformPredict(void *arg,const double *x);
void uniformFree(void *arg);
bestPrediction *bestPredictionModel(const double *sample,int numSample,int dimention);
void bestPredictionFree(bestPrediction *ctx);
void makeDammyData(double *new,const double *old,int numSample,int dimention,int newDimention,const var_info *vars);
int calcNewDimention(const var_info *vars,int limit);
bestPrediction *trainFromDB(const char *sql,const char *host,const char *db,const char *user,const char *password,const var_info *vars);
double bestPredictionProb(bestPrediction *ctx,const double *x);

void arrPrint(int *arr,int num);

#endif
#ifdef _cplusplus
}
#endif
