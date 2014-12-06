/**
 * @file string.c
 * @brief proccessing string utility
 * @author Y.Mitsui
 */

#include "libstandard.h"
char *trim(char *str){
	char *l,*r;
	for(l=str;isspace(l);l++) ;
	for(r=str+strlen(str);isspace(r);r--) ;
	*(r+1)='\0';
	return l;
}
