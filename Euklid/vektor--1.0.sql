create type vektor;

CREATE FUNCTION vektor_in(cstring,oid,integer)
    RETURNS vektor
    AS 'MODULE_PATHNAME','vektor_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vektor_out(vektor)
    RETURNS cstring
    AS 'MODULE_PATHNAME','vektor_out'
    LANGUAGE C IMMUTABLE STRICT;

Create Function vektor_in_modifier(cstring[])
    RETURNS integer
    AS 'MODULE_PATHNAME','vektor_in_modifier'
    LANGUAGE C IMMUTABLE STRICT;

Create Function vektor_out_modifier(integer)
    RETURNS cstring
    AS 'MODULE_PATHNAME','vektor_in_modifier'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mvektor(vektor,int,bool)
    RETURNS vektor
    AS 'MODULE_PATHNAME','mvektor'
    LANGUAGE C IMMUTABLE STRICT;


create type vektor(
internallength=VARIABLE,
input=vektor_in,
output=vektor_out,
typmod_in=vektor_in_modifier,
typmod_out=vektor_out_modifier
);

CREATE CAST(vektor AS vektor) WITH FUNCTION mvektor(vektor,int,bool) AS IMPLICIT;

Create Function euklidlen(vektor,vektor)
    RETURNS float
    AS 'MODULE_PATHNAME','euklidlen'
    LANGUAGE C IMMUTABLE STRICT;

Create OPERATOR <->(
leftarg= vektor,
rightarg= vektor,
procedure= euklidlen,
commutator= <->
);