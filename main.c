#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "src/headers/common.h"
#include "src/headers/serialize.h"

// yikes, dont do this kids
#define SO_REUSEPORT 15
int multiply(int a, int b);
int multiply_server_stub_unmarshal(ser_buff_t* server_recv_ser_buffer);
void multiply_server_stub_marshal(int res, ser_buff_t* server_send_ser_buffer);
void rpc_server_process_msg(ser_buff_t* server_recv_ser_buffer,
                            ser_buff_t* server_send_ser_buffer);

// @TODO: DISTRUBUTE INTO SEP FILES, POC MODE HERE
int multiply(int a, int b) {
  return a * b;
}

int multiply_server_stub_unmarshal(ser_buff_t* server_recv_ser_buffer) {
  int a,b;

  serlib_deserialize_data_string((char*)&a, server_recv_ser_buffer, sizeof(int));
  serlib_deserialize_data_string((char*)&b, server_recv_ser_buffer, sizeof(int));

  return multiply(a, b);
};

void multiply_server_stub_marshal(int res, ser_buff_t* server_send_ser_buffer) {
  serlib_serialize_data_string(server_send_ser_buffer, (char*)&res, sizeof(int));
};

void rpc_server_process_msg(ser_buff_t* server_recv_ser_buffer,
                            ser_buff_t* server_send_ser_buffer)
{
  int res = multiply_server_stub_unmarshal(server_recv_ser_buffer);
  
  multiply_server_stub_marshal(res, server_send_ser_buffer);
};

int main(int argc, char** argv) {
  int sock_udp_fd = 0,
      reply_msg_size = 0,
      addr_len,
      len,
      opt = 1;

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
  int addr_sock_opt_port = setsockopt(sock_udp_fd, SOL_SOCKET, SO_REUSEPORT, (char*)&opt, sizeof(opt));

  if (addr_sock_opt_addr < 0) {
    perror("setsockopt reuseaddr");
    exit(EXIT_FAILURE);
  }

  if (addr_sock_opt_port < 0) {
    perror("setsockopt reuseport");
    exit(EXIT_FAILURE);
  }

  if (bind(sock_udp_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
    printf("RPC ERROR:: Failed to bind to socket\n");
    exit(1);
  }

  // prepare server memory buffers to send and recv serialized data
  ser_buff_t *server_send_ser_buffer = NULL,
             *server_recv_ser_buffer = NULL;

  serlib_serialize_init_buffer_of_size(&server_recv_ser_buffer,
                                 MAX_RECV_SEND_BUFF_SIZE);

  serlib_serialize_init_buffer_of_size(&server_send_ser_buffer,
                                 MAX_RECV_SEND_BUFF_SIZE);

  printf("RPC Server is now listening on port %d...\n", RPC_SERVER_PORT);
READ:
  serlib_reset_buffer(server_recv_ser_buffer);

  // recv data from client into local buffer
  len = recvfrom(sock_udp_fd,
                 server_recv_ser_buffer->buffer,
                 serlib_get_buffer_length(server_recv_ser_buffer),
                 0, (struct sockaddr*)&client_addr,
                 &addr_len);

  printf("Number of bytes received: %d\n", len);

  // prepare the buffer to store the reply msg to be sent to client
  serlib_reset_buffer(server_send_ser_buffer);

  rpc_server_process_msg(server_recv_ser_buffer,
                         server_send_ser_buffer);

  goto READ;
  
  // close socket
  close(sock_udp_fd);

  return 0;
}

