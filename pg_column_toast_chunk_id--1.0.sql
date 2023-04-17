CREATE OR REPLACE FUNCTION pg_column_toast_chunk_id(anyelement)
RETURNS oid 
AS 'MODULE_PATHNAME', 'pg_column_toast_chunk_id'
LANGUAGE C;
