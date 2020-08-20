#include "src/headers/rpc.h"

int main(void) {
  while(true) {
    empman_rpc_handle_traffic();
  }
  
  return 0;
}

