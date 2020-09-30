#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"
#include "utils/array.h"

#define MAX_PARAMETRS 1024

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif



typedef struct {
	double a,b,c,d;
} Vektor;


PG_FUNCTION_INFO_V1(vektor_in);

Datum
vektor_in(PG_FUNCTION_ARGS) {

    char* str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid typelen=PG_GETARG_OID(1);
#endif

    int32 a=PG_GETARG_INT32(2);
  

//ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("second argument:\"%d\"",PG_GETARG_INT32(2))));



 	double      x,y,z,j;
	Vektor* result;

    if (sscanf(str, "(%lf,%lf,%lf,%lf)", &x, &y,&z,&j) != 4)
        ereport(ERROR,
        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for Vektor: \"%s\"",str)));

    result = (Vektor*)palloc(sizeof(Vektor));
    result->a=x; 
    result->b=y;   
    result->c=z;   
    result->d=j;   
	  

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(vektor_out);

Datum 
vektor_out(PG_FUNCTION_ARGS) {
    Vektor* complex = (Vektor*)PG_GETARG_POINTER(0);
    char* result;

    result = (char*)palloc(100);
    snprintf(result, 100, "(%g,%g,%g,%g)", complex->a, complex->b,complex->c,complex->d);
PG_RETURN_CSTRING(result);
}


PG_FUNCTION_INFO_V1(vektor_in_modifier);

Datum vektor_in_modifier(PG_FUNCTION_ARGS){

ArrayType *ta=PG_GETARG_ARRAYTYPE_P(0);
int32* tl;
int n;
int32 typmod;

tl=ArrayGetIntegerTypmods(ta,&n);

if(n==1){

	if(tl[0]<1 || tl[0]>MAX_PARAMETRS){
		ereport(ERROR,(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("Vector dimension %d in smaller than 1 or bigger than %d",
                tl[0],MAX_PARAMETRS)));
		
	}

typmod	= tl[0];
}
else{
	ereport(ERROR,(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("You can define only Vector dimension ")));
	typmod=-1;
}
	
PG_RETURN_INT32(typmod);
}


PG_FUNCTION_INFO_V1(vektor_out_modifier);

Datum vektor_out_modifier(PG_FUNCTION_ARGS){

int32 typmod=PG_GETARG_INT32(0);
char *res=(char*)palloc(32);

if(typmod>=0)
	snprintf(res,32,"(%d)",typmod);

else
	*res='\0';

PG_RETURN_CSTRING(res);
}


PG_FUNCTION_INFO_V1(mvektor);

Datum mvektor(PG_FUNCTION_ARGS){

Vektor* source=(Vektor*)PG_GETARG_DATUM(0);
int32 typmod=PG_GETARG_INT32(1);

if (typmod < 0)
    PG_RETURN_POINTER(source);

PG_RETURN_POINTER(NULL);
}
