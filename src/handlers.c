#include "headers/serialize.h"

void empman_rpc_handlers_employees_get_id(ser_buff_t* recv_buffer) {
  char* id;
  // deserialize request to get employee id
  serlib_deserialize_data_string(recv_buffer, (char*)&id, sizeof(char*));
  
  // query database with id


  // convert db response into data

  // create memory for employees linked list
  list_t* employees = (list_t*) malloc(sizeof(list_t));

  
  // convert data into employee linked list



  // serialize employees linked list   
};

