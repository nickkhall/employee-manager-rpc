#include "src/headers/rpc.h"

int main() {
  int is_running = 1;
  while(is_running == 1) {
    is_running = empman_rpc_handle_traffic();
  }
  
  return 0;
}

