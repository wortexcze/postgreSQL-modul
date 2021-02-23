MODULES=vektor
EXTENSION=vektor
DATA=vektor--1.0.sql
PG_CONFIG=pg_config
LIBS+= -lstdc++
LIBS+= -lngt 
PGXS:=$(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
