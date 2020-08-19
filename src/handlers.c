#include <stdlib.h>
#include <libpq-fe.h>

#include "headers/db.h"
#include "headers/serialize.h"

void empman_rpc_handlers_employees_get_id(ser_buff_t* recv_buffer) {
  // deserialize header to increment next counter for employee data deserialization
  //serlib_deserialize_data_string(recv_buffer, (char*));

  const char* id = (char*) malloc(sizeof(char) * 33);
  // deserialize request to get employee id
  serlib_deserialize_data_string(recv_buffer, id, sizeof(char*));
 
  // query database with id
  const char* const* query_params = &id;
  PGresult* db_response = empman_rpc_db_query_by_id(query_params);

  // convert db response into data

  // create memory for employees linked list
  list_t* employees = (list_t*) malloc(sizeof(list_t));

  
  // convert data into employee linked list



  // serialize employees linked list   
};

