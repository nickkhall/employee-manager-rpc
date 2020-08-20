#ifndef __EMP_MAN_RPC_EMPLOYEES_H__
#define __EMP_MAN_RPC_EMPLOYEES_H__

#include <libpq-fe.h>
#include <time.h>

#include "serialize.h"

extern const char* employee_labels_mini[];

typedef struct {
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

typedef struct {
  struct employee_t*      data;
  struct employee_list_node_t* next;
} list_node_t;

typedef struct {
  struct employee_list_node_t* head;
} list_t;

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer);

employee_t* empman_rpc_employees_push(employee_t* employee, char** data);

employee_t* empman_rpc_employees_populate(employee_t* employee, char** data);

employee_t* empman_rpc_employees_update(employee_t* employee, const char** data);

employee_t* empman_rpc_employees_remove(employee_t* employee, const char* id);

void empman_rpc_employees_destroy(employee_t* employee);

employee_t* empman_rpc_employees_convert(PGresult* res, const char* const* params, employee_t* employee);

void empman_rpc_employees_get_id(ser_buff_t* recv_buffer);

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
                             void(*serialize_fn_ptr)(void*, ser_buff_t* b));

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list.
 * ----------------------------------------------------------------------
 */
list_t* empman_rpc_employees_deserialize_list_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee list node.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_list_node_t(list_node_t* list_node, ser_buff_t* b, void (*serialize_fn_ptr)(void*, ser_buff_t* b));

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_deserialize_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list node.
 * ----------------------------------------------------------------------
 */
list_node_t* empman_rpc_employees_deserialize_list_node_t(ser_buff_t* b);

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

