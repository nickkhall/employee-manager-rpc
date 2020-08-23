#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "headers/utils.h"
#include "headers/serialize.h"

char* empman_rpc_utils_trim_whitespaces(char* str) {
  char* end;
  
  // trim leading space
  while(isspace(*str))
  	str++;
  
  if(*str == 0) // all spaces?
  	return str;
  
  // trim trailing space
  end = str + strnlen(str, 101) - 1;
  
  while(end > str && isspace(*end))
  	end--;
  
  // write new null terminator
  *(end + 1) = '\0';
  
  return str;
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_new(list_t* list, int elem_size, void (*freeFn)(void *)) {
  // make sure there is a size
  assert(elem_size > 0);
  
  // set the default values
  list->logical_length = 0;
  list->elem_size = elem_size;
  list->head = NULL;
  list->tail= NULL;

  // pass freeing function ptr
  list->freeFn = freeFn;
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_destroy(list_t* list) {
  // create current node pointer
  list_node_t* current_node;

  while(list->head != NULL) {
    // set current node to list head to start
    current_node = list->head;

    // call clean up pointer function if there is one
    if (list->freeFn) {
      list->freeFn(current_node->data);
    }

    // free up node's data
    free(current_node->data);
    // free up node's pointer
    free(current_node);
  }
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_prepend(list_t* list, void* element) {
  // allocate memory for node
  list_node_t* node = malloc(sizeof(list_node_t));

  // create memory for node data
  node->data = malloc(list->elem_size);
  // copy data to node
  memcpy(node->data, element, list->elem_size);

  // set the next of node to list head
  node->next = list->head;
  // set list head to current node
  list->head = node;

  // if first node
  if (!list->tail) {
    // set list / node tail to head
    list->tail = list->head;
  }

  // increment count of nodes
  list->logical_length++;
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_append(list_t* list, void* element) {
  // create memory for node
  list_node_t* node = malloc(sizeof(list_node_t));
  // create memory for node data
  node->data = malloc(list->elem_size);
  // set node pointer to NULL
  node->next = NULL;

  // copy the data into the node
  memcpy(node->data, element, list->elem_size);

  // if this is first node in list
  if (list->logical_length == 0) {
    list->head = node;
    list->tail = node;
  // else, we have other nodes, so append
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  // increment count of nodes
  list->logical_length++;
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
int empman_utils_list_get_size(list_t* list) {
  return list->logical_length;
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_iterate(list_t* list, bool (*list_iterator)(void *)) {
  assert(list_iterator != NULL);

  list_node_t* node = list->head;
  bool result = true;
  while(node != NULL && result) {
    result = list_iterator(node->data);
    node = node->next;
  }
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_get_head(list_t* list, void* element, bool should_remove) {
};

/*
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 *
 * ------------------------------------------------------
 */
void empman_utils_list_get_tail(list_t* list, void* element) {
};
