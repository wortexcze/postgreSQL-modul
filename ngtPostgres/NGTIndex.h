#ifndef INDEX_H
#define INDEX_H
#include "NGT/Capi.h"

NGTIndex ngt;
NGTProperty property;
NGTError err;


int created=0;

PG_FUNCTION_INFO_V1(createNGT);
Datum createNGT(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(querryNGT);
Datum querryNGT(PG_FUNCTION_ARGS);


#endif