#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"
#include "utils/array.h"
#include "vektor.h"

int parsersgtring(char* a) {
    int len = strlen(a);
    char lbracket = 0, rbracket = 0;

    for (int i = 0; i < len; i++) {
        if (i == 0 && a[i] != '(')
            return 0;
        else if (a[i] == '(') {
            if (lbracket < 1) {
                lbracket++;
            }
            else return 0;
        }
        else if (a[i] == ')') {
            if (rbracket < 1) {
                rbracket++;
            }
            else return 0;
        }
        ///parse number
        else if (isdigit(a[i]) || a[i] == ',' || a[i] == '.') {
            if (a[i] == ',') {
                if (isdigit(a[i - 1]) && isdigit(a[i + 1])) {
                    continue;
                }
                else
                    return 0;
            }
            else if (a[i] == '.') {
                if (isdigit(a[i - 1]) && isdigit(a[i + 1])) {
                    continue;
                }
                else
                    return 0;
            }
            else if (isdigit(a[i])) {
                continue;
            }
            else
                return 0;
        }
        else
            return 0;
    }

    return 1;
}

int coutnumber(char *a) {
    int len = strlen(a);
    int count = 0,pom=0;

    for (int i = 1; i < len; i++) {
        if (isdigit(a[i])) {
            pom = 1;
        }
        else if (a[i] == '.')
            continue;
        else if (a[i] == ',') {
            count++;
            pom = 0;
        }
    }
    if (pom == 1)
        count++;

    return count;

}

void removeSpaces(char* str)
{   int count = 0;
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; 
    str[count] = '\0';
}

double get_double(const char* str)
{
    while (*str && !(isdigit(*str) || ((*str == '-' || *str == '+') && isdigit(*(str + 1)))))
        str++;

    return strtod(str, NULL);
}


void fromstrdouble(char* a, double* b,int *del)
{
    int len = strlen(a);
    char buffer[65],pom=0;
    int start = 0, end = 0;
    (*del) = 0;
    for (int i = 1; i < len; i++) {  
        if (isdigit(a[i])|| a[i]=='.') {
            pom = 1;
            if (start < 1) {
                start = i;
            }
        }
        else {
            end = i;
            memcpy(buffer, &a[start], end-start);
            start = end = 0;
            b[(*del)] = get_double(buffer);
            (*del)++;
        }
    }
}

Datum vektor_in(PG_FUNCTION_ARGS) {

    char* str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid typelen=PG_GETARG_OID(1);
#endif

    int32 a=PG_GETARG_INT32(2);

removeSpaces(str);

if(!parsersgtring(str))
ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("error in syntax:\"%s\"",str)));

    int len=coutnumber(str);
    Vektor*   result = (Vektor*)palloc(VARHDRSZ+len+sizeof(double));
    SET_VARSIZE(result,VARHDRSZ+len);

    fromstrdouble(str,result->data,&len);
    PG_RETURN_POINTER(result);
}


Datum vektor_out(PG_FUNCTION_ARGS) {
    Vektor* complex = (Vektor*)PG_GETARG_POINTER(0);
	char* m="nejaky vypis";
PG_RETURN_CSTRING(m);
}

Datum vektor_in_modifier(PG_FUNCTION_ARGS){

ArrayType *tPava=PG_GETARG_ARRAYTYPE_P(0);
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
	typmod=0;
}
	
PG_RETURN_INT32(typmod);
}

Datum vektor_out_modifier(PG_FUNCTION_ARGS){

int32 typmod=PG_GETARG_INT32(0);
char *res=(char*)palloc(32);

if(typmod>0)
	snprintf(res,32,"(%d)",typmod);

else
	*res='\0';

PG_RETURN_CSTRING(res);
}
