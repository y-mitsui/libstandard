static GSList* __subGetFileList(apr_pool_t *pool,GSList* r,const char *path){
	DIR *dir;
	struct dirent *dp;
	if((dir=opendir(path))==NULL) return NULL;

		while((dp=readdir(dir))){
			switch(dp->d_type){
			case DT_DIR:
				r=__subGetFileList(pool,r,apr_pstrcat(pool,path,"/",dp->d_name,NULL));
				break;
			case DT_REG:
				r=g_slist_append(r,dp->d_name);
				break;
			}
	}
	closedir(dir);
	return r;
}
GSList* getFileList(const char *dir){
	apr_pool_t *pool=NULL;
	apr_initialize();
	apr_pool_create(&pool,NULL);
	GSList* r=__subGetFileList(pool,NULL,dir);
	apr_pool_destroy(pool);
	return r;
}
