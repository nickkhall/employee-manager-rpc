#include <stdlib.h>
#include <libpq-fe.h>

#include "headers/employees.h"
#include "headers/db.h"
#include "headers/serialize.h"

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer) {
  // deserialize header to increment next counter for employee data deserialization
  //serlib_deserialize_data(recv_buffer, (char*));

  const char* id = (char*) malloc(sizeof(char) * 33);
  // deserialize request to get employee id
  serlib_deserialize_data(recv_buffer, id, sizeof(char*));
 
  // query database with id
  const char* const* query_params = &id;
  PGresult* db_response = empman_rpc_db_query_by_id(query_params);

  // create memory for employees linked list
  list_t* employees = (list_t*) malloc(sizeof(list_t));
  if (!employees) {
    printf("ERROR:: RPC - Failed to allocate memory for employess linked list in empman_rpc_handlers_employees_get_id\n");
    free(recv_buffer);
    exit(1);
  }

  // @HERE: WIP, need to create linked list with new generic linked list type
  // convert db response into data
  int rows = PQntuples(res); 
  for (int r = 0; r < rows; r++) {
    empman_rpc_db_convert_pq_data(*(employee);
  }

  
  // convert data into employee linked list



  // serialize employees linked list   
};

