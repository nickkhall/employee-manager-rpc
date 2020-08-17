#ifndef __EMP_MAN_REST_SERIALIZE__
#define __EMP_MAN_REST_SERIALIZE__

#define SERIALIZE_BUFFER_DEFAULT_SIZE 100

#include "employee.h"
#include "common.h"

typedef enum {
  RPC_REQ,
  RPC_REPLY,
  RPC_AUTH
} rpc_enum_t;

typedef struct ser_buff_t {
  char* buffer;
  int size;
  int next;
} ser_buff_t;

typedef struct ser_header_t {
  unsigned int tid;
  unsigned int rpc_proc_id;
  rpc_enum_t msg_type;
  unsigned int payload_size;
} ser_header_t;

typedef struct client_param_t {
  unsigned int recv_buff_size;
  ser_buff_t*  recv_ser_b;
} client_param_t;

/*
 * ------------------------------------------------------
 * function: serlib_init_buffer
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_init_buffer(ser_buff_t** b);

/*
 * ------------------------------------------------------
 * function: serlib_init_buffer_of_size
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_init_buffer_of_size(ser_buff_t** b, int size);

/*
 * ------------------------------------------------------
 * function: serlib_get_header_size
 * ------------------------------------------------------
 * Returns size of serialized header.
 * ------------------------------------------------------
 */
unsigned int serlib_get_header_size(void);

/*
 * --------------------------------------------------------------------
 * function: serlib_buffer_skip
 * --------------------------------------------------------------------
 * params  :
 *         > buffer    - ser_buff_t*
 *         > skip_size - int
 * --------------------------------------------------------------------
 * Skips a section of memory in the buffer's buffer. 
 * (In/Decrements the next pointer)
 * --------------------------------------------------------------------
 */
void serlib_buffer_skip(ser_buff_t* b, unsigned long int skip_size);

/*
 * ---------------------------------------------------
 * function: serlib_reset_buffer
 * ---------------------------------------------------
 * params  : b - ser_buff_t*
 * ---------------------------------------------------
 * Resets a buffer. (Sets ->next to 0)
 * ---------------------------------------------------
 */
int serlib_reset_buffer(ser_buff_t* b);

/*
 * ---------------------------------------------------
 * function: serlib_get_buffer_length
 * ---------------------------------------------------
 * params  : b - ser_buff_t*
 * ---------------------------------------------------
 * Frees the memory and destroys a buffer type.
 * ---------------------------------------------------
 */
int serlib_get_buffer_length(ser_buff_t* b);

/*
 * -----------------------------------------------------
 * function: serlib_get_buffer_data_size
 * -----------------------------------------------------
 * params  : b - ser_buff_t*
 * -----------------------------------------------------
 * Gets data size of serialized buffer.
 * -----------------------------------------------------
 */
int serlib_get_buffer_data_size(ser_buff_t* b);

/*
 * -----------------------------------------------------
 * function: serlib_copy_in_buffer_by_size
 * -----------------------------------------------------
 * params  : b - ser_buff_t*
 * -----------------------------------------------------
 * 
 * -----------------------------------------------------
 */
void serlib_copy_in_buffer_by_size(ser_buff_t* client_send_ser_buffer, int size, char* value, int offset);

/*
 * --------------------------------------------
 * function: serlib_free_buffer
 * --------------------------------------------
 * params  : b - ser_buff_t*
 * --------------------------------------------
 * Frees the memory and destroys a buffer type.
 * --------------------------------------------
 */
void serlib_free_buffer(ser_buff_t* b);

/*
 * ------------------------------------------------------------------------
 * function: serlib_serialize_data_string
 * ------------------------------------------------------------------------
 * params  : 
 *           > b      - ser_buff_t**
 *           > data   - char*
 *           > nbytes - int
 * ------------------------------------------------------------------------
 * Serializes string data to a given valid serialized string buffer.
 * ------------------------------------------------------------------------
 */
void serlib_serialize_data_string(ser_buff_t* b, char* data, int nbytes);

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_data_string
 * ----------------------------------------------------------------------
 * params  :
 *         > dest - char*
 *         > b    - ser_buff_t*
 *         > size - int
 * ----------------------------------------------------------------------
 * Deserializes a buffers' string buffer.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_string(char* dest, ser_buff_t* b, int size);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a time_t.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_data_time_t(time_t* dest, ser_buff_t*b, int size);

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a time_t.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_time_t(time_t* dest, ser_buff_t*b, int size);

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
 * function: serlib_employee_t_wrapper
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deerializes a employee list node.
 * ----------------------------------------------------------------------
 */
void serlib_employee_t_wrapper(void* obj, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_employee_t
 * ----------------------------------------------------------------------
 * params  :
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee_t.
 * ----------------------------------------------------------------------
 */
void serlib_employee_t(employee_t* employee, ser_buff_t* b);

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : 
 *         > employee - employee_t*
 *         > b        - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee_t.
 * ----------------------------------------------------------------------
 */
employee_t* serlib_deserialize_employee_t(ser_buff_t* b);

#endif
