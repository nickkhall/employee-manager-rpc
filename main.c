#include "src/headers/rpc.h"

int main() {
  while(1) {
    empman_rpc_handle_traffic();
  }
  
  return 0;
}

