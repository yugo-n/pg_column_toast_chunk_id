# pg_column_toast_chunk_id

PostgreSQL extension to get the chunk id of a TOASTed column.
The extension is compatible with PostgreSQL 13, 14, and 15.

```
pg_column_toast_chunk_id(any) RETURNS oid
```

`pg_column_toast_chunk_id` returns a chunk ID of a TOASTed value, or NULL if the value is not TOASTed. This function is useful when you would like to know which columns are actually TOASTed. You may want to know it for infering how much updates on the original table will affect to its toast table specifically with regard to auto VACUUM. This function is also useful to identify a problematic row when an error like below occurs;
```
ERROR:  unexpected chunk number ... (expected ...) for toast value
```

## Example

```
postgres=# \d val
               Table "public.val"
 Column | Type | Collation | Nullable | Default
--------+------+-----------+----------+---------
 t      | text |           |          |


postgres=# select length(t), pg_column_size(t), pg_column_compression(t), pg_column_toast_chunk_id(t), tableoid from val;
 length | pg_column_size | pg_column_compression | pg_column_toast_chunk_id | tableoid
--------+----------------+-----------------------+--------------------------+----------
      3 |              4 |                       |                       |    16388
   3000 |             46 | pglz                  |                       |    16388
  32000 |            413 | pglz                  |                       |    16388
    305 |            309 |                       |                       |    16388
  64000 |          64000 |                       |                 16393 |    16388
(5 rows)


postgres=# select chunk_id, chunk_seq from pg_toast.pg_toast_16388;
 chunk_id | chunk_seq
----------+-----------
    16393 |         0
    16393 |         1
    16393 |         2
 (snip)
    16393 |        30
    16393 |        31
    16393 |        32
(33 rows)
```

## Installation

To install `pg_column_toast_chunk_id`, execute this in the module's directory:

```shell
make install
```

If you installed PostgreSQL from rpm or deb, you will need the devel package (for example, postgresql14-devel or postgresql-server-dev-14).

> **Important:** Don't forget to set the `PG_CONFIG` variable (`make PG_CONFIG=...`) or the `PATH` to the `pg_config` command in case you want to use `pg_column_toast_chunk_id` on a non-default or custom build of PostgreSQL. Read more [here](https://wiki.postgresql.org/wiki/Building_and_Installing_PostgreSQL_Extension_Modules).

And, execute CREATE EXTENSION comand.

```
CREATE EXTENSION pg_column_toast_chunk_id;
```
