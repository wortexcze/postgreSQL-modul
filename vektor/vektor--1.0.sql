create type vektor;
CREATE FUNCTION vektor_in(cstring)
    RETURNS vektor
    AS 'MODULE_PATHNAME','vektor_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vektor_out(vektor)
    RETURNS cstring
    AS 'MODULE_PATHNAME','vektor_out'
    LANGUAGE C IMMUTABLE STRICT;


create type vektor(
internallength=32,
input=vektor_in,
output=vektor_out
);