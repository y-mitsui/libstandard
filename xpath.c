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

char *xpathOneContent(htmlDocPtr doc,const char *xpath){
	GSList* links=query_xpath(doc,"//table[@class='db_prof_table']/tr[1]/td");
	char *p=(char*)xmlNodeGetContent(links->data);
	g_slist_free(links);
	return p;
}
