#include <stdlib.h>
#include <libpq-fe.h>
#include <serialize.h>
#include <db.h>

#include "../include/employees.h"
#include "../include/utils.h"

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer, ser_buff_t* send_buffer) {
  // deserialize header to increment next counter for employee data deserialization
  //serlib_deserialize_data(recv_buffer, (char*));

  const char* id = (char*) malloc(sizeof(char) * 33);
  // deserialize request to get employee id
  serlib_deserialize_data(recv_buffer, (void*)id, sizeof(char*));
 
  // query database with id
  const char* const* query_params = &id;
  const char* SQL_INFO = getenv("SQL_INFO");
  const char* query = "SELECT * FROM employees WHERE id = $1 OR first = $1 OR last = $1";
  PGresult* db_response = libdbc_db_query_by_id(query_params, SQL_INFO, query);
  if (!db_response) {
    printf("ERROR:: RPC - Failed to query database.\n");
    free(recv_buffer);
    free(send_buffer);
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

    libdbc_db_convert_pq_data(*(data + r), db_response, r);
  }

  // create memory for employee linked list
  list_t* employees = (list_t*) malloc(sizeof(list_t));
  if (!employees) {
    printf("ERROR:: RPC - Failed to allocate memory for employees in empman_rpc_employees_get_id\n");
    for (int i = 0; i < 11; i++) {
      free(*(data + i));
    }

    free(data);
    free(recv_buffer);
    exit(1);
  }

  // create generic employees linked list
  empman_utils_list_new(employees, sizeof(employee_t), NULL); 

  // iterate through number of employees from response,
  // and create an employee_t employee
  // and append it to the generic list
  for (int r = 0; r < rows; r++) {
    employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
    employee = empman_rpc_employees_employee_create(*(data + r));

    empman_utils_list_append(employees, employee);
  }

  // @TODO: REFACTOR / ABSTRACT [POC MODE]
  // create serialized header
  // serialized header shite
  int SERIALIZED_HDR_SIZE = sizeof(ser_header_t); // temporary
  serlib_buffer_skip(send_buffer, SERIALIZED_HDR_SIZE);

  ser_header_t* ser_header = (ser_header_t*) malloc(sizeof(ser_header_t));
  if (!ser_header) {
    printf("ERROR:: REST - Failed to allocate memory for serialized header\n");
    free(send_buffer);
    exit(1);
  }

  ser_header->tid = 10;
  ser_header->rpc_proc_id = 55;
  ser_header->rpc_call_id = 3;
  ser_header->payload_size = 0;

  // serialize generic employee linked list into send buffer
  serlib_serialize_list_t(employees, send_buffer, (void*) empman_rpc_employees_serialize_employee_t_wrapper);

  // now that we have payload size
  // resume serialized header shite
  ser_header->payload_size = serlib_get_buffer_data_size(send_buffer) - SERIALIZED_HDR_SIZE;

  serlib_copy_in_buffer_by_offset(send_buffer,
                                sizeof(ser_header->tid),
                                (char*)&ser_header->tid,
                                0); 

  serlib_copy_in_buffer_by_offset(send_buffer,
                                sizeof(ser_header->rpc_proc_id),
                                (char*)&ser_header->rpc_proc_id,
                                sizeof(ser_header->tid)); 

  serlib_copy_in_buffer_by_offset(send_buffer,
                                sizeof(ser_header->rpc_call_id),
                                (char*)&ser_header->rpc_call_id,
                                sizeof(ser_header->tid) + sizeof(ser_header->rpc_proc_id)); 

  serlib_copy_in_buffer_by_offset(send_buffer,
                                sizeof(ser_header->payload_size),
                                (char*)&ser_header->payload_size,
                                (sizeof(ser_header->rpc_call_id)
                                + sizeof(ser_header->tid)
                                + sizeof(ser_header->rpc_proc_id))); 
}

/*
 *
 *
 *
 *
 */
employee_t* empman_rpc_employees_employee_initialize(void) {
  employee_t* employee = (employee_t*) malloc(sizeof(employee_t));

  employee->id =        (char*) malloc(sizeof(char) * 33);
  employee->first =     (char*) malloc(sizeof(char) * 51);
  employee->last =      (char*) malloc(sizeof(char) * 51);
  employee->email =     (char*) malloc(sizeof(char) * 101);
  employee->address =   (char*) malloc(sizeof(char) * 76);
  employee->phone =     (char*) malloc(sizeof(char) * 51);
  employee->start =     (time_t) malloc(sizeof(time_t));
  employee->gender =    (char*) malloc(sizeof(char) * 7);
  employee->ethnicity = (char*) malloc(sizeof(char) * 51);
  employee->title =     (char*) malloc(sizeof(char) * 51);
  employee->salary =    (char*) malloc(sizeof(int*));

  return employee;
}

/*
 *
 *
 *
 *
 */
employee_t* empman_rpc_employees_employee_create(char** data) {
  if (!data) {
    printf("ERROR:: RPC - Invalid pointer for data in empman_rpc_employees_employee_create\n");
    exit(1);
  }

  employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
  if (!employee) {
    for (int d = 0; d < 11; d++) {
      printf("ERROR:: RPC - Failed to allocate memory for employee in empman_rpc_employees_employee_create\n");
      free(*(data + d));
      free(data);
      exit(1);
    }
  }

  employee->id =        (char*) *data;
  employee->first =     (char*) *(data + 1);
  employee->last =      (char*) *(data + 2);
  employee->email =     (char*) *(data + 3);
  employee->address =   (char*) *(data + 4);
  employee->phone =     (char*) *(data + 5);
  employee->start =     (time_t) *(data + 6);
  employee->ethnicity = (char*) *(data + 7);
  employee->gender =    (char*) *(data + 8);
  employee->title =     (char*) *(data + 9);
  employee->salary =    (int*) (*(data + 10));

  return employee;
}

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
}

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_serialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_serialize_employee_t(employee_t* employee, ser_buff_t* b)
{
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
  serlib_serialize_data(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data(b, (char*)employee->title,     sizeof(char) * 51);

  if (employee->salary) {
    serlib_serialize_data(b, (char*)employee->salary, sizeof(int));
  } else {
    serlib_serialize_data(b, (char*)&sentinel, sizeof(unsigned long int));
  }
}

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
}

