/**
 * @file xpath.c
 * @brief xpath utility of libxml2
 * @author Y.Mitsui
 */

#include "libstandard.h"

GSList* query_xpath(htmlDocPtr doc,const char *query){
	int i;
	xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
	if (!ctx) return NULL;
    
	xmlXPathObjectPtr xpobj = xmlXPathEvalExpression((xmlChar *)query, ctx);
	if (!xpobj) return NULL;

	xmlNodeSetPtr nodes = xpobj->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
	GSList *r=NULL;
	for (i = 0; i < size; ++i) {
		if (!xmlXPathNodeSetIsEmpty(nodes)) {
			xmlNodePtr node = xmlXPathNodeSetItem(nodes, i);
			r=g_slist_append(r,node);
		}
	}

	xmlXPathFreeObject(xpobj);
	xmlXPathFreeContext(ctx);
	return r;
}

char *xpathOneContent(apr_pool_t *pool,htmlDocPtr doc,const char *xpath){
	GSList* links=query_xpath(doc,xpath);
	if(!links) return NULL;
	char *p=(char*)xmlNodeGetContent(links->data);
	if(pool){
		apr_pool_userdata_set(p,"libstandard-xpathOneContent",(apr_status_t (*)(void *))free,pool);
	}
	g_slist_free(links);
	return p;
}
