Datum createNGT(PG_FUNCTION_ARGS){
Vektor* complex = (Vektor*)PG_GETARG_POINTER(0);
char* str = PG_GETARG_CSTRING(1);

if(!created){
extern "C"{
ngt_set_property_dimension(property,complex->delka,err);
ngt_set_property_object_type_float(property,err);
ngt_set_property_distance_type_cosine(property,err);
ngt_append_index_as_float(ngt,complex->data,complex->delka,err);
}
}}


Datum querryNGT(PG_FUNCTION_ARGS){
ngt_create_index(ngt,16,err);
created=1;
}


