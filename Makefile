MODULE_big  = pg_column_toast_chunk_id
OBJS        = pg_column_toast_chunk_id.o $(WIN32RES)

EXTENSION = pg_column_toast_chunk_id
DATA = pg_column_toast_chunk_id--1.0.sql
PGFILEDESC = "pg_column_toast_chunk_id - get chunk id of a TOASTed column"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
