#ifndef __EMP_MAN_RPC__
#define __EMP_MAN_RPC__

#include <serialize.h>

typedef enum {
  EMP_MAN_RPC_EMPLOYEES_GET_ID
} empman_rpc_calls;

int* empman_rpc_init();

void empman_rpc_init_buffers(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void empman_rpc_process_traffic(ser_buff_t** recv_buffer, ser_buff_t** send_buffer);

void empman_rpc_handle_traffic();

#endif

