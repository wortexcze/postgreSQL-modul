#include "postgres.h"
#include <string.h>
#include <math.h>
#include "fmgr.h"
#include "funcapi.h"
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

void fromdoubletostring(const double* a, char* b,const int size) {
  strcat(b,"(");
    for (int i = 0; i < size; i++) {
	char but[10];
        double pom = a[i];
	if((i+1)==size)
		sprintf(but,"%.2f)",pom);
	else
        	sprintf(but,"%.2f,",pom);

        strcat(b, but);
    }
      
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
    char buffer[65];
    int start = 0, end = 0;
    (*del) = 0;
    for (int i = 1; i < len; i++) {  
        if (isdigit(a[i])|| a[i]=='.') {
           // pom = 1;
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

double euklidlen(Vektor *a,Vektor *b){
double len=0,ret;
double pom;
int i;

for(i=0;i<(a->delka);i++){
pom=a->data[i]-b->data[i];
len+=(pom*pom);
}

ret=sqrt(len);

return ret;

}

Datum vektor_in(PG_FUNCTION_ARGS) {

    char* str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid typelen=PG_GETARG_OID(1);
#endif

    int32 a=PG_GETARG_INT32(2);
    int len;

removeSpaces(str);

if(!parsersgtring(str))
ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("error in syntax:\"%s\"",str)));

    len=coutnumber(str);
    Vektor*   result = (Vektor*)palloc(VARHDRSZ+len*sizeof(double)+sizeof(int));
    SET_VARSIZE(result,VARHDRSZ+len*sizeof(double)+sizeof(int));

    fromstrdouble(str,result->data,&len);
    result->delka=len;
    PG_RETURN_POINTER(result);
}


Datum vektor_out(PG_FUNCTION_ARGS) {
    Vektor* complex = (Vektor*)PG_GETARG_POINTER(0);
	char* m=(char*)palloc(100);
	*m='\0';
	//sprintf(m,"(%g,%g,%g,%g)",complex->data[0],complex->data[1],complex->data[2],complex->data[3]);
	//sprintf(m,"(%d)",complex->delka);
	fromdoubletostring(complex->data,m,complex->delka);
	
	PG_RETURN_CSTRING(m);
}

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

Datum mvektor(PG_FUNCTION_ARGS){
	Vektor    *source =(Vektor*) PG_GETARG_DATUM(0);
	int32		typmod = PG_GETARG_INT32(1);
	bool		isExplicit = PG_GETARG_BOOL(2);


	if (source->delka==typmod)
		PG_RETURN_POINTER(source);

	if (!isExplicit)
	{    if (source->delka!=typmod)
		ereport(ERROR,(errcode(ERRCODE_STRING_DATA_RIGHT_TRUNCATION),errmsg("You declare (%d) dimension, but you must need (%d)dimension vektor",source->delka,typmod)));
	}
}

Datum nearest(PG_FUNCTION_ARGS){

   FuncCallContext* funcctx;
    VektorSearch* counterContext;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldContext;

        funcctx = SRF_FIRSTCALL_INIT();

        oldContext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        counterContext = palloc(sizeof(VektorSearch));
        counterContext->maxlen = PG_GETARG_INT32(2);
        counterContext->currentlen = -1;
        counterContext->a =(Vektor **) palloc(sizeof(Vektor*) * counterContext->maxlen);
        counterContext->b =(double *) palloc(sizeof(double) * counterContext->maxlen);


        funcctx->user_fctx = counterContext;
        MemoryContextSwitchTo(oldContext);
    }

    funcctx = SRF_PERCALL_SETUP();
    counterContext = (VektorSearch*)funcctx->user_fctx;

    if (funcctx->call_cntr<1) {

        double sc=euklidlen((Vektor*)PG_GETARG_POINTER(0),(Vektor*)PG_GETARG_POINTER(1));

        if (counterContext->currentlen< counterContext->maxlen) {
            counterContext->currentlen++;
            counterContext->b[counterContext->currentlen]=sc;
            counterContext->a[counterContext->currentlen]=(Vektor*)PG_GETARG_POINTER(0);
		SRF_RETURN_NEXT(funcctx,(Datum)PG_GETARG_POINTER(0));
        }

        else {
            double max = -1;
            int index = -1;
            for (int i = 0; i <= counterContext->currentlen; i++) {
                if (max < counterContext->b[i]) {
                    max = counterContext->b[i];
                    index = i;
                }
            }

            if (index != -1) {
                counterContext->b[index] = sc;
                counterContext->a[index]=(Vektor*)PG_GETARG_POINTER(0);
		SRF_RETURN_NEXT(funcctx,(Datum)PG_GETARG_POINTER(0));
            }
        }

        
    }
    else {
        SRF_RETURN_DONE(funcctx);
    }
    PG_RETURN_VOID();
}

