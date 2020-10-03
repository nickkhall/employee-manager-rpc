#include "include/rpc.h"

int main() {
  for(;;) {
    empman_rpc_handle_traffic();
  }
  
  return 0;
}

