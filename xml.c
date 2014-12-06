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
