#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif



typedef struct {
	double a,b, c, d;
} Vektor;

PG_FUNCTION_INFO_V1(vektor_in);

Datum
vektor_in(PG_FUNCTION_ARGS) {
    char* str = PG_GETARG_CSTRING(0);
    double      x,y,z,j;
    Vektor* result;

    if (sscanf(str, "(%lf,%lf,%lf,%lf)", &x, &y,&z,&j) != 4)
        ereport(ERROR,
        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for complex: \"%s\"",
                str)));

    result = (Vektor*)palloc(sizeof(Vektor));
    result->a = x;
    result->b = y;
    result->c = z;
    result->d = j;
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