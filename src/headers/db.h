#ifndef __EMP_MAN_RPC_DB_H__
#define __EMP_MAN_RPC_DB_H__

#include <libpq-fe.h>

extern const char SEARCH_BY_ID_QUERY[];

PGresult* empman_rpc_db_query(PGconn* conn,
                  const char* query,
                  const char* const* queryParams,
                  const int num_of_queries);

PGresult* empman_rpc_db_query_by_id(const char* const* query_params);

void empman_rpc_db_clean_up(PGconn* conn, PGresult* res);

void empman_rpc_db_disconnect(PGconn* conn);

/*
 * ------------------------------------------
 * function: empman_rpc_db_convert_pq_data
 * ------------------------------------------
 * params  : 
 *          > conn - pointer to PGconn type
 *          > res  - pointer to PGresult type
 * ------------------------------------------
 * Destroys a response,
 * and a connection to a database
 * ------------------------------------------
 */
char** empman_rpc_db_convert_pq_data(char** data_pointer, PGresult* res, const int row);

#endif

