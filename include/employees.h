#ifndef __EMP_MAN_RPC_EMPLOYEES_H__
#define __EMP_MAN_RPC_EMPLOYEES_H__

#include <libpq-fe.h>
#include <time.h>

#include "serialize.h"

extern const char* employee_labels_mini[];

typedef struct _employee_t {
  char* id;
  char* first;
  char* last;
  char* email;
  char* address;
  char* phone;
  time_t start;
  char* gender;
  char* ethnicity;
  char* title;
  int* salary;
} employee_t;

/*
 *
 *
 *
 *
 */
void empman_rpc_employees_get_id(ser_buff_t* recv_buffer, ser_buff_t* send_buffer);

/*
 *
 *
 *
 *
 */
employee_t* empman_rpc_employees_employee_initialize(void);

/*
 *
 *
 *
 *
 */
employee_t* empman_rpc_employees_employee_create(char** data);

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
void empman_rpc_employees_serialize_employee_t_wrapper(void* obj, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_serialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_serialize_employee_t(employee_t* employee, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* empman_rpc_employees_deserialize_employee_t(list_node_t* data, ser_buff_t* b);

#endif

