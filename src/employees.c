#include <stdlib.h>
#include <libpq-fe.h>

#include "headers/employees.h"
#include "headers/serialize.h"
#include "headers/db.h"

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer) {
  // deserialize header to increment next counter for employee data deserialization
  //serlib_deserialize_data(recv_buffer, (char*));

  const char* id = (char*) malloc(sizeof(char) * 33);
  // deserialize request to get employee id
  serlib_deserialize_data(recv_buffer, (void*)id, sizeof(char*));
 
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

  int rows = PQntuples(db_response); 
  int cols = PQnfields(db_response);

  char*** data = (char***) malloc(sizeof(char*) * rows); 
  if (!data) {
    printf("ERROR:: RPC - Failed to allocate memory for data for postgres response in empman_rpc_employees_get_id\n");
    free((char*)id);
    free(recv_buffer);
    // @TODO: add app clean up function to clean up memory
    exit(1);
  }
  // convert db response into data
  for (int r = 0; r < rows; r++) {
    *(data + r) = (char**) malloc(sizeof(char*) * cols);
    if (!*(data + r)) {
      printf("ERROR:: RPC - Failed to allocate memory for data transfer in empman_rpc_employees_get_id\n");
      free(data);
      exit(1);
    }

    empman_rpc_db_convert_pq_data(*(data + r), db_response, r);
  }

  
  // convert data into employee linked list


  // serialize employees linked list   
};

/*
 * +--------------------------------------+
 * |          Generic Wrappers            |
 * +--------------------------------------+
 */

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_serialize_employee_t_wrapper
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Generic wrapper function for serializing an employee.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_serialize_employee_t_wrapper(void* obj, ser_buff_t* b) {
  empman_rpc_employees_serialize_employee_t(obj, b);
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_serialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_serialize_employee_t(employee_t* employee, ser_buff_t* b) {
  // if this is a sentinel section, return null
  unsigned int sentinel = 0xFFFFFFFF;
  if (!employee) {
    serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_serialize_data(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->title,     sizeof(char) * 51);

  if (employee->salary) {
    serlib_serialize_data(b, (char*)employee->salary, sizeof(int));
  } else {
    serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned long int));
  }
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* empman_rpc_employees_deserialize_list_t(ser_buff_t* b) {
  unsigned int sentinel = 0;

  serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned int));

  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serlib_buffer_skip(b, -1 * sizeof(unsigned long int));

  employee_t* employee = calloc(1, sizeof(employee_t));

  serlib_serialize_data(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->title,     sizeof(char) * 51);

  serlib_serialize_data(b, (char*)&sentinel, sizeof(int));

  if (sentinel == 0xFFFFFFFF) {
    employee->salary = NULL;
  } else {
    serlib_buffer_skip(b, -1 * sizeof(unsigned long int));
    employee->salary = calloc(1, sizeof(int));
    serlib_serialize_data(b, (char*) employee->salary, sizeof(int));
  }

  return employee;
};

