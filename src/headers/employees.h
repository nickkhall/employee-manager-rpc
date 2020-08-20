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

/*
 * ----------------------------------------------------------------------
 * function: serlib_list_t
 * ----------------------------------------------------------------------
 * params  :
 *         > list - list_t*
 *         > b             - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee list.
 * ----------------------------------------------------------------------
 */
void serlib_list_t(list_t* list,
                             ser_buff_t* b,
                             void(*serialize_fn_ptr)(void*, ser_buff_t* b));

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee list.
 * ----------------------------------------------------------------------
 */

list_t* serlib_deserialize_list_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_list_node_t
 * ----------------------------------------------------------------------
 * params  : 
 *         > list_node - list_node_t*
 *         > b                  - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee list node.
 * ----------------------------------------------------------------------
 */
void serlib_list_node_t(list_node_t* list_node,
                                  ser_buff_t* b,
                                  void (*serialize_fn_ptr)(void*, ser_buff_t* b));

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deerializes a employee list node.
 * ----------------------------------------------------------------------
 */
list_node_t* serlib_deserialize_list_node_t(ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_wrapper_deserialize_list
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deerializes a employee list node.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_wrapper_deserialize_list(void* obj, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employees_t
 * ----------------------------------------------------------------------
 * params  :
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee_t.
 * ----------------------------------------------------------------------
 */
void empman_rpc_employees_wrapper_serialize_emp(employee_t* employee, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: empman_rpc_employeesdeserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : 
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee_t.
 * ----------------------------------------------------------------------
 */
employee_t* empman_rpc_employees_deserialize_emp(ser_buff_t* b);

#endif

