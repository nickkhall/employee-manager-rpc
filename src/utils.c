#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "headers/utils.h"

char* empman_rpc_utils_trim_whitespaces(char* str){
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
}
