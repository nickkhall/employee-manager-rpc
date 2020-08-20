#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "headers/postgres_info.h"
#include "headers/db.h"

const char SEARCH_BY_ID_QUERY[] = "SELECT * FROM employees WHERE id = $1 OR first = $1 OR last = $1";

/*
 * -----------------------------------------------------------------------------
 * function: empman_rpc_db_query
 * -----------------------------------------------------------------------------
 * params:   
 *         > query          - constant char*
 *         > query_params   - constant pointer pointing to constant char pointer
 *         > num_of_queries - constant integer
 * -----------------------------------------------------------------------------
 * returns:  pointer to PGresult type
 * -----------------------------------------------------------------------------
 */
PGresult* empman_rpc_db_query(PGconn* conn,
                  const char* query,
                  const char* const* query_params,
                  const int num_of_queries)
{
  if (!conn) {
    printf("ERROR:: RPC - Failed to connect to postgres db.\n");
    exit(1);
  }

  //PGresult* res = (PGresult*) malloc(sizeof(struct PGresult));
  PGresult* res = PQexec(conn, "BEGIN");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    empman_rpc_db_disconnect(conn);
    exit(1);
  }

  PQclear(res);

  res = PQexecParams(conn,
                    query, num_of_queries,
                    NULL, query_params, NULL,
                    NULL, 0);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("ERROR:: RPC - %s\n", PQerrorMessage(conn));
    PQclear(res);
    empman_rpc_db_disconnect(conn);
    exit(1);
  }

  int pq_size = 0;
  for (int pq = 0; pq < PQnfields(res); pq++) {
    pq_size += PQfsize(res, pq); 
  }

  PGresult* response = (PGresult*) malloc(sizeof(res));
  if (!response) {
    printf("ERROR:: RPC - Failed to allocate memory for postgres response in empman_rpc_db_query\n");
    empman_rpc_db_disconnect(conn);
    free((char*)query_params);
    exit(1);
  }

  memcpy(response, res, pq_size);

  // disconnect from db
  empman_rpc_db_disconnect(conn);

  return res;
};


/*
 * --------------------------------------------------------------------
 * function: empman_rpc_db_query_by_id
 * --------------------------------------------------------------------
 * params:   query_params - constant pointer pointing to constant char*
 * --------------------------------------------------------------------
 * returns:  pointer to PGresult type
 * --------------------------------------------------------------------
 */
PGresult* empman_rpc_db_query_by_id(const char* const* query_params) {
  // check for query params
  if (!query_params) {
    printf("ERROR:: RPC - Query params data is invalid in empman_rpc_db_query_by_id");
    exit(1);
  }

  // connect to db
  PGconn* conn = PQconnectdb(SQL_INFO);
  if (!conn) {
    printf("ERROR:: RPC - Failed to connect to postgres db.\n");
    exit(1);
  }

  // query db for employee
  PGresult* res = empman_rpc_db_query(conn,
                            SEARCH_BY_ID_QUERY,
                            query_params, 1);

  // check status of postman response
  if (!res) {
    printf("ERROR:: RPC - Failed to get PQ response from empman_rpc_db_query in empman_rpc_db_query_by_id\n");
    PQclear(res);
    free(res);
    empman_rpc_db_disconnect(conn);
    exit(1);
  }

  return res;
};

/*
 *
 *
 *
 *
 *
 */
PGresult* empman_rpc_db_query_post(PGresult* res, const char* const* query_params) {
  PGconn* conn = PQconnectdb(SQL_INFO);

  if (!conn) {
    printf("ERROR:: RPC - Failed to connect to postgres db\n");
    exit(1);
  }

  if (!query_params) {
    printf("ERROR:: RPC - Query data is invalid in empman_rpc_db_query_post");
    empman_rpc_db_disconnect(conn);
    exit(1);
  }

  res = empman_rpc_db_query(conn,
                SEARCH_BY_ID_QUERY,
                query_params, 1);

  if (!res) {
    printf("ERROR:: RPC - Failed to get PQ response from empman_rpc_db_query in empman_rpc_db_query_post\n");
    PQclear(res);
    free(res);
    empman_rpc_db_disconnect(conn);
    exit(1);
  }

  return res;
};

/*
 * ---------------------------------------
 * function: empman_rpc_db_disconnect
 * ---------------------------------------
 * params  : conn - pointer to PGconn type
 * ---------------------------------------
 * Destroys a connection to a database.
 * ---------------------------------------
 */
void empman_rpc_db_disconnect(PGconn* conn) {
  PQfinish(conn);
};

/*
 * ------------------------------------------
 * function: empman_rpc_db_clean_up
 * ------------------------------------------
 * params  : 
 *          > conn - pointer to PGconn type
 *          > res  - pointer to PGresult type
 * ------------------------------------------
 * Destroys a response,
 * and a connection to a database
 * ------------------------------------------
 */
void empman_rpc_db_clean_up(PGconn* conn, PGresult* res) {
  PQclear(res);
  empman_rpc_db_disconnect(conn);
};

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
char** empman_rpc_db_convert_pq_data(char** data_pointer, PGresult* res, const int row) {
  for (int col = 0; col < 11; col++) {
    const char* current_pq = PQgetvalue(res, row, col);
    unsigned long int current_pq_length = strlen(current_pq);
    char* current_string = (char*) malloc(sizeof(char) * (current_pq_length + 1));

    strcpy(current_string, current_pq);
    *(data_pointer + col) = current_string;
 }

  return data_pointer;
};

