#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "headers/serialize.h"
#include "headers/employee.h"

/*
 * ------------------------------------------------------
 * function: serlib_init_buffer
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_init_buffer(ser_buff_t** b) {
  // create memory for serialized buffer type
  (*b) = (ser_buff_t*) calloc(1, sizeof(ser_buff_t));

  // create memory for serialized buffer's buffer
  (*b)->buffer = calloc(1, SERIALIZE_BUFFER_DEFAULT_SIZE);

  // set buffer size to default size
  (*b)->size = SERIALIZE_BUFFER_DEFAULT_SIZE;

  // set buffer's next segment
  (*b)->next = 0;
};

/*
 * ------------------------------------------------------
 * function: serlib_init_buffer_of_size
 * ------------------------------------------------------
 * params  : b - ser_buff_t**
 * ------------------------------------------------------
 * Initializes the serialized buffer type.
 * ------------------------------------------------------
 */
void serlib_init_buffer_of_size(ser_buff_t** b, int size) {
  (*b) = (ser_buff_t*) calloc(1, sizeof(ser_buff_t));
  (*b)->buffer = calloc(1, size);
  (*b)->size = size;
  (*b)->next = 0;
};

/*
 * ------------------------------------------------------
 * function: serlib_get_header_size
 * ------------------------------------------------------
 * Returns size of serialized header.
 * ------------------------------------------------------
 */
unsigned int serlib_get_header_size(void) {
  ser_header_t ser_header;
  return sizeof(ser_header.tid)
         + sizeof(ser_header.rpc_proc_id)
         + sizeof(ser_header.msg_type)
         + sizeof(ser_header.payload_size);
};

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
void serlib_buffer_skip(ser_buff_t* b, unsigned long int skip_size) {
  // if the skip_size is above 0,
  // and the buffer has access to the needed memory
  if (b->next + skip_size > 0 &&
      b->next + skip_size < b->size
  ) {
    // skip the buffer
    // (adjust the next pointer)
    b->next += skip_size;
  }
};

/*
 * ---------------------------------------------------
 * function: serlib_reset_buffer
 * ---------------------------------------------------
 * params  : b - ser_buff_t*
 * ---------------------------------------------------
 * Resets a buffer. (Sets ->next to 0)
 * ---------------------------------------------------
 */
int serlib_reset_buffer(ser_buff_t* b) {
  b->next = 0;
};

/*
 * ----------------------------------------------------
 * function: serlib_get_buffer_length
 * ----------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------
 * Gets length of serialized buffer.
 * ----------------------------------------------------
 */
int serlib_get_buffer_length(ser_buff_t* b) {
  return b->size;
};

/*
 * -------------------------------------------------------
 * function: serlib_get_buffer_data_size
 * -------------------------------------------------------
 * params  : b - ser_buff_t*
 * -------------------------------------------------------
 * Gets data size of serialized buffer.
 * -------------------------------------------------------
 */
int serlib_get_buffer_data_size(ser_buff_t* b) {
  return b->next;
};

/*
 * -----------------------------------------------------
 * function: serlib_copy_in_buffer_by_size
 * -----------------------------------------------------
 * params  : b - ser_buff_t*
 * -----------------------------------------------------
 * 
 * -----------------------------------------------------
 */
void serlib_copy_in_buffer_by_size(ser_buff_t* client_send_ser_buffer, int size, char* value, int offset) {
  if (offset > client_send_ser_buffer->size) {
    printf("%s(): ERROR:: REST - Attempted to write outside of buffer limits\n", __FUNCTION__);
    return;
  }

  memcpy(client_send_ser_buffer->buffer + offset, value, size);
};

/*
 * --------------------------------------------
 * function: serlib_free_buffer
 * --------------------------------------------
 * params  : b - ser_buff_t*
 * --------------------------------------------
 * Frees the memory and destroys a buffer type.
 * --------------------------------------------
 */
void serlib_free_buffer(ser_buff_t* b) {
  free(b->buffer);
  free(b);
};

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
void serlib_serialize_data_string(ser_buff_t* b, char* data, int nbytes) {
  if (b == NULL) assert(0);

  ser_buff_t* buff = (ser_buff_t*)(b);
  // get total available size of buffer
  int available_size = buff->size - buff->next;
  // resize flag used for resizing buffer
  int should_resize = 0;

  // if we don't have enough memory for data in buffer
  while(available_size < nbytes) {
    // increase (multiply) buffer size by 2
    buff->size = buff->size * 2;

    // update total available size
    available_size = buff->size - buff->next;

    // set should resize flag
    should_resize = 1;
  }

  // else we have enough memory for data in buffer
  if (should_resize == 0) {
    // copy data from src to buffer's buffer (b->buffer)
    memcpy((char*)buff->buffer + buff->next, data, nbytes);

    // increase the buffers next memory to nbytes
    buff->next += nbytes;

    return;
  }

  // resize the buffer
  buff->buffer = realloc(buff->buffer, buff->size);

  // copy data to buffer's buffer (b->buffer)
  memcpy((char*)buff->buffer + buff->next, data, nbytes);

  // increase buffer's next memory by nbtyes
  buff->next += nbytes;

  return;
};

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
void serlib_deserialize_data_string(ser_buff_t* b, char* dest, int size) {
  if (!b || !b->buffer) assert(0);
  if (!size) return;
  if ((b->size - b->next) < size) assert(0);

  // copy data from dest to string buffer
  memcpy(dest, b->buffer + b->next, size);

  // increment the buffer's next pointer
  b->next += size;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_data_int
 * ----------------------------------------------------------------------
 * params  :
 *         > dest - int*
 *         > b    - ser_buff_t*
 *         > size - int
 * ----------------------------------------------------------------------
 * Deserializes a buffers' string buffer for an integer.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_int(ser_buff_t* b, int* dest, int size) {
  if (!b || !b->buffer) assert(0);
  if (!size) return;
  if ((b->size - b->next) < size) assert(0);

  // copy data from dest to string buffer
  memcpy(dest, (int*)b->buffer + b->next, size);

  // increment the buffer's next pointer
  b->next += size;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_serialize_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_serialize_data_time_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_deserialize_data_time_t(time_t* dest, ser_buff_t*b, int size) {
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a buffers' employee_t buffer.
 * ----------------------------------------------------------------------
 */
void serlib_list_t(list_t* list,
                             ser_buff_t* b,
                             void(*serialize_fn_ptr)(void*, ser_buff_t* b))
{
  // if this is a sentinel section, return null
  if (!list) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_list_node_t(list->head, b, serialize_fn_ptr);
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_list_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list.
 * ----------------------------------------------------------------------
 */
list_t* serlib_deserialize_list_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data_string(b, (char*)&sentinel,  sizeof(unsigned long int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  list_t* list = calloc(1, sizeof(list_t));
  list->head = serlib_deserialize_list_node_t(b);

  return list;
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes a employee list node.
 * ----------------------------------------------------------------------
 */
void serlib_list_node_t(list_node_t* list_node, ser_buff_t* b, void (*serialize_fn_ptr)(void*, ser_buff_t* b)) {
  // if this is a sentinel section, return null
  if (!list_node) {
    unsigned int sentinel = 0xFFFFFFFF;
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serialize_fn_ptr(list_node->data, b);
  serlib_list_node_t(list_node->next, b, serialize_fn_ptr);
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_list_node_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes a employee list node.
 * ----------------------------------------------------------------------
 */
list_node_t* serlib_deserialize_list_node_t(ser_buff_t* b) {
  // set sentintal to default
  unsigned int sentinel = 0;

  // unmarshall buffer to check for sentinel
  serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));

  // if this is a sentinel section, return null
  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  list_node_t* list_node = calloc(1, sizeof(list_node_t));
  
  list_node->data = serlib_deserialize_employee_t(b);
  list_node->next = serlib_deserialize_list_node_t(b);

  return list_node;
};

/*
 * +--------------------------------------+
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |          Generic Wrappers            |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * +--------------------------------------+
 */

/*
 * ----------------------------------------------------------------------
 * function: serlib_employee_t_wrapper
 * ----------------------------------------------------------------------
 * params  : 
 *         > obj - void*
 *         > b   - ser_buff_t*
 * ----------------------------------------------------------------------
 * Generic wrapper function for serializing an employee.
 * ----------------------------------------------------------------------
 */
void serlib_employee_t_wrapper(void* obj, ser_buff_t* b) {
  serlib_employee_t(obj, b);
};

/*
 * +--------------------------------------+
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |          Employee Specific           |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * |                                      |
 * +--------------------------------------+
 */

/*
 * ----------------------------------------------------------------------
 * function: serlib_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Serializes an employee.
 * ----------------------------------------------------------------------
 */
void serlib_employee_t(employee_t* employee, ser_buff_t* b) {
  // if this is a sentinel section, return null
  unsigned int sentinel = 0xFFFFFFFF;
  if (!employee) {
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));
    return;
  }

  serlib_serialize_data_string(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data_string(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data_string(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data_string(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data_string(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data_string(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->title,     sizeof(char) * 51);

  if (employee->salary) {
    serlib_serialize_data_string(b, (char*)employee->salary, sizeof(int));
  } else {
    serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned long int));
  }
};

/*
 * ----------------------------------------------------------------------
 * function: serlib_deserialize_employee_t
 * ----------------------------------------------------------------------
 * params  : b - ser_buff_t*
 * ----------------------------------------------------------------------
 * Deserializes an employee.
 * ----------------------------------------------------------------------
 */
employee_t* serlib_deserialize_employee_t(ser_buff_t* b) {
  unsigned int sentinel = 0;

  serlib_serialize_data_string(b, (char*)&sentinel, sizeof(unsigned int));

  if (sentinel == 0xFFFFFFFF) {
    return NULL;
  }

  serlib_buffer_skip(b, -1 * sizeof(unsigned long int));

  employee_t* employee = calloc(1, sizeof(employee_t));

  serlib_serialize_data_string(b, (char*)employee->id,        sizeof(char) * 33);
  serlib_serialize_data_string(b, (char*)employee->first,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->last,      sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->email,     sizeof(char) * 101);
  serlib_serialize_data_string(b, (char*)employee->address,   sizeof(char) * 76);
  serlib_serialize_data_string(b, (char*)employee->phone,     sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->start,     sizeof(time_t));
  serlib_serialize_data_string(b, (char*)employee->gender,    sizeof(char) * 7);
  serlib_serialize_data_string(b, (char*)employee->ethnicity, sizeof(char) * 51);
  serlib_serialize_data_string(b, (char*)employee->title,     sizeof(char) * 51);

  serlib_serialize_data_string(b, (char*)&sentinel, sizeof(int));

  if (sentinel == 0xFFFFFFFF) {
    employee->salary = NULL;
  } else {
    serlib_buffer_skip(b, -1 * sizeof(unsigned long int));
    employee->salary = calloc(1, sizeof(int));
    serlib_serialize_data_string(b, (char*) employee->salary, sizeof(int));
  }

  return employee;
};

