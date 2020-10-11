#ifndef VEKTOR_H
#define VEKTOR_H

#define MAX_PARAMETRS 1024
#define DEFAULT_PARAMETRS 4

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct {
	int length;
	char data[1];
} Vektor;


int parsersgtring(char* a);
int coutnumber(char *a);
void removeSpaces(char* str);

PG_FUNCTION_INFO_V1(vektor_in);
Datum vektor_in(PG_FUNCTION_ARGS); 

PG_FUNCTION_INFO_V1(vektor_out);
Datum vektor_out(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(vektor_in_modifier);
Datum vektor_in_modifier(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(vektor_out_modifier);
Datum vektor_out_modifier(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(mvektor);
Datum mvektor(PG_FUNCTION_ARGS);
#endif