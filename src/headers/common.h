#ifndef __RPC_EMP_MAN_COMMON__
#define __RPC_EMP_MAN_COMMON__

#define RPC_SERVER_PORT 8741

#define MAX_SEND_BUFF_SIZE 2048
#define MAX_RECV_BUFF_SIZE 2048

#include "serialize.h"

typedef enum {
  RPC_REQ,
	RPC_REPLY,
	RPC_AUTH
} rpc_enum_t;

typedef struct rpc_header {
  unsigned int tid;
	unsigned int rpc_proc_id;
	rpc_enum_t msg_type;
	unsigned int payload_size;
} rpc_header;

typedef struct client_param_t {
  unsigned int recv_buff_size;
	ser_buff_t*  recv_ser_b;
} client_param_t;

unsigned int rpc_header_size();

#endif

