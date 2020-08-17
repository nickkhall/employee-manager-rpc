#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "src/headers/common.h"
#include "src/headers/serialize.h"

#define MULTIPLY_ID 55

// yikes, dont do this kids
//#define SO_REUSEPORT 15
/*
 * RPC FUNCTIONS (MOCK / POC)
 */
int multiply(int a, int b);
int multiply_server_stub_unmarshal(ser_buff_t* recv_buffer);
void multiply_server_stub_marshal(int res, ser_buff_t* send_buffer);


// void rpc_server_process_msg(ser_buff_t* send_buffer);
// rpc_callback rpc_callback_array[];

//unsigned int rpc_ser_header_size() {
//  rpc_ser_header_t rpc_ser_header;
//
//  return sizeof(rpc_ser_header.tid)
//         + sizeof(rpc_ser_header.rpc_proc_id)
//         + sizeof(rpc_ser_header.msg_type)
//         + sizeof(rpc_ser_header.payload_size);
//};

// @TODO: DISTRUBUTE INTO SEP FILES, POC MODE HERE
int multiply(int a, int b) {
  return a * b;
}

int multiply_server_stub_unmarshal(ser_buff_t* recv_buffer) {
  int a,b;

  serlib_deserialize_data_string((char*)&a, recv_buffer, sizeof(int));
  serlib_deserialize_data_string((char*)&b, recv_buffer, sizeof(int));

  return multiply(a, b);
};

void multiply_server_stub_marshal(int res, ser_buff_t* send_buffer) {
  serlib_serialize_data_string(send_buffer, (char*)&res, sizeof(int));
};

void rpc_server_process_msg(ser_buff_t* recv_buffer,
                            ser_buff_t* send_buffer) {
  ser_header_t* rpc_ser_header = (ser_header_t*) malloc(sizeof(struct ser_header_t));
  if (!rpc_ser_header) {
    printf("ERROR:: RPC - Failed to allocate memory for rpc_ser_header\n");
    free(recv_buffer);
    free(send_buffer);
    exit(1);
  }

  serlib_deserialize_data_string(send_buffer, (char*)&rpc_ser_header->rpc_proc_id,  sizeof(rpc_ser_header->rpc_proc_id));
  serlib_deserialize_data_string(send_buffer, (char*)&rpc_ser_header->payload_size, sizeof(rpc_ser_header->payload_size));

  if (rpc_ser_header->rpc_proc_id == MULTIPLY_ID) {
    int res = multiply_server_stub_unmarshal(recv_buffer);

    multiply_server_stub_marshal(res, send_buffer);
  }

  //printf("RPC - rpc_ser_header.rpc_proc_id: %d\n", rpc_ser_header->rpc_proc_id);
  //return rpc_callback_array[rpc_ser_header.rpc_proc_id](send_buffer);
  
};

int main(int argc, char** argv) {
  int sock_udp_fd = 0,
      reply_msg_size = 0,
      addr_len,
      len,
      opt = 1;

  ser_buff_t* recv_buffer = NULL;
  ser_buff_t* send_buffer = NULL;

  serlib_init_buffer_of_size(&send_buffer, MAX_RECV_BUFF_SIZE);
  serlib_init_buffer_of_size(&recv_buffer, MAX_RECV_BUFF_SIZE);

  struct sockaddr_in server_addr,
                     client_addr;

  sock_udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock_udp_fd == -1) {
    printf("RPC ERROR:: Failed to create socket\n");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(RPC_SERVER_PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  addr_len = sizeof(struct sockaddr);

  int addr_sock_opt_addr = setsockopt(sock_udp_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

  if (addr_sock_opt_addr < 0) {
    perror("setsockopt reuseaddr");
    exit(EXIT_FAILURE);
  }

  int binded = bind(sock_udp_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

  if (binded == -1) {
    printf("RPC ERROR:: Failed to bind to socket\n");
    exit(1);
  }

  printf("RPC Server is now listening on port %d...\n", RPC_SERVER_PORT);

READ:
  serlib_reset_buffer(recv_buffer);

  // step 4
  // recv data from client into local buffer
  len = recvfrom(sock_udp_fd,
                 recv_buffer->buffer,
                 serlib_get_buffer_length(recv_buffer),
                 0, (struct sockaddr*)&client_addr,
                 &addr_len);

  printf("RPC server replied with %d bytes\n", len);

  serlib_reset_buffer(send_buffer);

  rpc_server_process_msg(recv_buffer,
                         send_buffer);

  // step 8
  // send the serialized result to client
  len = sendto(sock_udp_fd, send_buffer->buffer,
              serlib_get_buffer_length(send_buffer),
              0, (struct sockaddr*)&client_addr,
              sizeof(struct sockaddr));

  //serlib_free_buffer(send_buffer);
  //serlib_free_buffer(recv_buffer);
  //serlib_reset_buffer(recv_buffer);

  goto READ;
  
  // close socket
  close(sock_udp_fd);

  return 0;
}

