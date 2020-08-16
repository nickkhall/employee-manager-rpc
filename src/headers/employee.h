#ifndef __EMP_MAN_EMPLOYEE_H__
#define __EMP_MAN_EMPLOYEE_H__

#include <libpq-fe.h>
#include <time.h>

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

employee_t* employee_push(employee_t* employee, char** data);

employee_t* employee_populate(employee_t* employee, char** data);

employee_t* employee_update(employee_t* employee, const char** data);

employee_t* employee_remove(employee_t* employee, const char* id);

void employee_destroy(employee_t* employee);

employee_t* employee_convert(PGresult* res, const char* const* params, employee_t* employee);

#endif

