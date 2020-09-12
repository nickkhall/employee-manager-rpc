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

employee_t* empman_rpc_employees_push(employee_t* employee, char** data);

employee_t* empman_rpc_employees_populate(employee_t* employee, char** data);

employee_t* empman_rpc_employees_update(employee_t* employee, const char** data);

employee_t* empman_rpc_employees_remove(employee_t* employee, const char* id);

void empman_rpc_employees_destroy(employee_t* employee);

employee_t* empman_rpc_employees_convert(PGresult* res, const char* const* params, employee_t* employee);

employee_t* empman_rpc_employees_employee_create(char** data);

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer, ser_buff_t* send_buffer);

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
void empman_rpc_employees_serialize_employee_t_wrapper(void* obj, ser_buff_t* b);

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
employee_t* empman_rpc_employees_deserialize_employee_t(ser_buff_t* b);

#endif

