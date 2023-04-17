#include "postgres.h"

#include "funcapi.h"
#include "utils/lsyscache.h"
#include "access/detoast.h"

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(pg_column_toast_chunk_id);

/*
 * Return the chunk id of the TOASTed value.
 * Return NULL for unTOASTed value.
 */
Datum
pg_column_toast_chunk_id(PG_FUNCTION_ARGS)
{
	int			typlen;
	struct varlena *attr;
	struct varatt_external toast_pointer;

	/* On first call, get the input type's typlen, and save at *fn_extra */
	if (fcinfo->flinfo->fn_extra == NULL)
	{
		/* Lookup the datatype of the supplied argument */
		Oid			argtypeid = get_fn_expr_argtype(fcinfo->flinfo, 0);

		typlen = get_typlen(argtypeid);
		if (typlen == 0)		/* should not happen */
			elog(ERROR, "cache lookup failed for type %u", argtypeid);

		fcinfo->flinfo->fn_extra = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
													  sizeof(int));
		*((int *) fcinfo->flinfo->fn_extra) = typlen;
	}
	else
		typlen = *((int *) fcinfo->flinfo->fn_extra);

	if (typlen != -1)
		PG_RETURN_NULL();

	attr = (struct varlena *) DatumGetPointer(PG_GETARG_DATUM(0));

	if (!VARATT_IS_EXTERNAL_ONDISK(attr))
		PG_RETURN_NULL();

	VARATT_EXTERNAL_GET_POINTER(toast_pointer, attr);

	PG_RETURN_OID(toast_pointer.va_valueid);
}
