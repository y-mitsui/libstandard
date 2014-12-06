/**
 * @file mathematics.c
 * @brief mathematics relation utility
 * @author Y.Mitsui
 */
#include "libstandard.h"

/******************************************************************************/
/*!	@brief Grant unique random number whose value is less than specified value.

	@param[out]		res		num unique number
	@param[in]		num		number you want
	@param[in]		limit	maxium value
******************************************************************************/
void uniqRandum(int *res,int num,int limit){
	int i,j,tmp;
	for(i=0;i<num;){
		tmp=rand()%limit;
		for(j=0;j<i;j++)
			if(res[j]==tmp) break;
		if(j==i){
			res[i]=tmp;
			i++;
		}
	}
}
