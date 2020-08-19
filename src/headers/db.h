#ifndef __EMP_MAN_RPC_DB_H__
#define __EMP_MAN_RPC_DB_H__

#include <libpq-fe.h>

#include "employee.h"

extern const char SEARCH_BY_ID_QUERY[];

PGresult* empman_rpc_db_query(PGconn* conn,
                  PGresult* res,
                  const char* query,
                  const char* const* queryParams,
                  const int num_of_queries);

PGresult* empman_rpc_db_query_by_id(PGresult* res, const char* const* query_params);

void empman_rpc_db_clean_up(PGconn* conn, PGresult* res);

void empman_rpc_db_disconnect(PGconn* conn);

#endif

