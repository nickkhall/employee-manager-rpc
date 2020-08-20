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

  // @HERE: WIP, need to create linked list with new generic linked list type
  // convert db response into data
  int rows = PQntuples(res); 
  for (int r = 0; r < rows; r++) {
    empman_rpc_db_convert_pq_data(*(employee);
  }

  
  // convert data into employee linked list



  // serialize employees linked list   
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_list_t(list_t* list,
                             ser_buff_t* b,
                             void(*serialize_fn_ptr)(void*, ser_buff_t* b))
{
  // if this is a sentinel section, return null
  if (!list) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  empman_rpc_employees_list_node_t(list->head, b, serialize_fn_ptr);
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list.
 * ----------------------------------------------------------------------
 */
list_t* empman_rpc_employees_deserialize_list_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data(b, (char*)&sentinel,  sizeof(unsigned long int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  list_t* list = calloc(1, sizeof(list_t));
  list->head = empman_rpc_employees_deserialize_list_node_t(b);

  return list;
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee list node.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_list_node_t(list_node_t* list_node, ser_buff_t* b, void (*serialize_fn_ptr)(void*, ser_buff_t* b)) {
  // if this is a sentinel section, return null
  if (!list_node) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serialize_fn_ptr(list_node->data, b);
  empman_rpc_employees_list_node_t(list_node->next, b, serialize_fn_ptr);
};

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list node.
 * ----------------------------------------------------------------------
 */
list_node_t* empman_rpc_employees_deserialize_list_node_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  list_node_t* list_node = calloc(1, sizeof(list_node_t));
  
  list_node->data = empman_rpc_employees_deserialize_employee_t(b);
  list_node->next = empman_rpc_employees_deserialize_list_node_t(b);

  return list_node;
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
 * +--------------------------------------+
 * |          Employee Specific           |
 * +--------------------------------------+
 */

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
 * function: empman_rpc_employees_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* empman_rpc_employees_deserialize_employee_t(ser_buff_t* b) {
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

