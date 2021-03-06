/**
 * @file file.c
 * @brief file and directory control utility
 * @author Y.Mitsui
 */

#include "libstandard.h"

void arrPrint(int *arr,int num){
	int i;
	printf("[ ");
	for(i=0;i<num;i++) printf("%d ",arr[i]);
	printf(" ]");
}
static GSList* __subGetFileList(apr_pool_t *pool,GSList* r,const char *path){
	DIR *dir;
	struct dirent *dp;
	char buf[1024];
	if((dir=opendir(path))==NULL) return NULL;

		while((dp=readdir(dir))){
			if(*dp->d_name=='.') continue;
			switch(dp->d_type){
			case DT_DIR:
				r=__subGetFileList(pool,r,apr_pstrcat(pool,path,"/",dp->d_name,NULL));
				break;
			case DT_REG:
				snprintf(buf,sizeof(buf),"%s/%s",path,dp->d_name);
				r=g_slist_append(r,strdup(buf));
				break;
			}
	}
	closedir(dir);
	return r;
}
/******************************************************************************/
/*!	@brief Get list of file name in directry designated

	@param[in]		dir	directory name searched
	@return			Success:file name list of char pointer
					Fail:NULL
******************************************************************************/
GSList* getFileList(const char *dir){
	apr_pool_t *pool=NULL;
	apr_initialize();
	apr_pool_create(&pool,NULL);
	GSList* r=__subGetFileList(pool,NULL,dir);
	apr_pool_destroy(pool);
	return r;
}
