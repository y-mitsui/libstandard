#ifndef LIBSTD_H
#define LIBSTD_H 1

#include <stdio.h>
enum DB_VALUE_TYPE{
	D_TYPE_INT,
	D_TYPE_STRING
};
typedef struct{
	enum DB_VALUE_TYPE type;
	char *value;
}DB_VALUE;

#endif
