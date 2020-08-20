#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "headers/rpc.h"
#include "headers/serialize.h"
#include "headers/employees.h"
#include "headers/sockets.h"

int* empman_rpc_init() {
  // create socket
  int* sock_udp_fd = (int*) malloc(sizeof(int));
  if (!sock_udp_fd) {
    printf("ERROR:: RPC - Failed to allocate memory for socket in empman_rpc_init\n");
    return NULL;
  }

  sock_udp_fd = socklib_socket_create();
  if (*sock_udp_fd == -1) {
    printf("ERROR:: RPC - Failed to create socket in empman_rpc_init\n");
    exit(1);
  }

  // create server socket
  struct sockaddr_in* server_addr = socklib_socket_build_sock_addr_in(sock_udp_fd,
                                                                 AF_INET,
                                                                 RPC_SERVER_PORT);
  // bind to rpc port
  int binded = bind(*sock_udp_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in*));
  if (binded == -1) {
    printf("RPC ERROR:: Failed to bind to socket\n");
    exit(1);
  }

  // print running message to screen
  printf("- Employee Manager - \nRPC - Server is now listening on port %d...\n", RPC_SERVER_PORT);

  return sock_udp_fd;
};

void empman_rpc_init_buffers(ser_buff_t* recv_buffer, ser_buff_t* send_buffer)
{
  serlib_init_buffer_of_size(&send_buffer, MAX_RECV_BUFF_SIZE);
  serlib_init_buffer_of_size(&recv_buffer, MAX_RECV_BUFF_SIZE);
};


int empman_rpc_process_traffic(ser_buff_t* recv_buffer, ser_buff_t* send_buffer)
{
  ser_header_t* rpc_ser_header = (ser_header_t*) malloc(sizeof(struct ser_header_t));
  if (!rpc_ser_header) {
    printf("ERROR:: RPC - Failed to allocate memory for rpc_ser_header\n");
    free(recv_buffer);
    free(send_buffer);
    return -1;
  }

  serlib_deserialize_data(recv_buffer, (char*)&rpc_ser_header->tid,          sizeof(rpc_ser_header->tid));
  serlib_deserialize_data(recv_buffer, (char*)&rpc_ser_header->rpc_proc_id,  sizeof(rpc_ser_header->rpc_proc_id));
  serlib_deserialize_data(recv_buffer, (char*)&rpc_ser_header->rpc_call_id,     sizeof(rpc_ser_header->msg_type));
  serlib_deserialize_data(recv_buffer, (char*)&rpc_ser_header->payload_size, sizeof(rpc_ser_header->payload_size));

  // @TODO: update to use `rpc_call_id` instead of `rpc_proc_id`
  switch (rpc_ser_header->rpc_proc_id) {
    case EMP_MAN_RPC_EMPLOYEES_GET_ID:
      empman_rpc_employees_get_id(recv_buffer);
      break;
    default:
      break;
  }

  return 0;
};



void empman_rpc_handle_traffic()
{
  // create client and server sockets
  struct sockaddr_in* server_addr = (sockaddr_in*) malloc(sizeof(sockaddr_in));
  struct sockaddr_in client_addr;
  int addr_len = sizeof(struct sockaddr);

  // initialize rpc sockets
  int* sock_udp_fd = empman_rpc_init(server_addr);

  // create and initialize send/recv buffers
  ser_buff_t* recv_buffer = NULL;
  ser_buff_t* send_buffer = NULL;
  empman_rpc_init_buffers(recv_buffer, send_buffer);

  // reset recv buffer
  serlib_reset_buffer(recv_buffer);

  // recv data from request into local buffer
  int len = recvfrom(*sock_udp_fd, recv_buffer->buffer,
                     serlib_get_buffer_length(recv_buffer),
                     0, (struct sockaddr*)&client_addr,
                     &addr_len);

  // print status
  printf("RPC server recieved %d bytes\n", len);

  // reset send buffer
  serlib_reset_buffer(send_buffer);

  // process request
  empman_rpc_process_traffic(recv_buffer,
                            send_buffer);

  // send the serialized result to client
  len = sendto(sock_udp_fd, send_buffer->buffer,
              serlib_get_buffer_length(send_buffer),
              0, (struct sockaddr*)&client_addr,
              sizeof(struct sockaddr));

  // reset send buffer
  serlib_reset_buffer(send_buffer);
};
