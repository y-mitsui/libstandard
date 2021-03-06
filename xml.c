/**
 * @file xml.c
 * @brief XML accessing functions
 * @author Y.Mitsui
 */

#include "libstandard.h"

xmlNodePtr xmlNodeGetChild(xmlNodePtr parent,int no){
	xmlNodePtr cur;
	int i;
	for(i=0,cur=parent->children;cur;cur=cur->next){
		if(cur->type!=XML_ELEMENT_NODE) continue;
		if(i==no) return cur;
		i++;
	}
	return NULL;
}
char* xmlNodeGetContentP(apr_pool_t *pool,xmlNodePtr node){
	char *p=(char*)xmlNodeGetContent(node);
	if(pool){
		apr_pool_userdata_set(p,"libstandard-xmlNodeGetContentP",(apr_status_t (*)(void *))free,pool);
	}
	return p;
	
}
