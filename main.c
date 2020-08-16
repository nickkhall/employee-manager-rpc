#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "src/headers/common.h"
#include "src/headers/serialize.h"

// yikes, dont do this kids
#define SO_REUSEPORT 15

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

  if (setsockopt(sock_udp_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt) < 0)) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(sock_udp_fd, SOL_SOCKET, SO_REUSEPORT, (char*)&opt, sizeof(opt) < 0)) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  if (bind(sock_udp_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
    printf("RPC ERROR:: Failed to bind to socket\n");
    exit(1);
  }

  ser_buff_t *server_send_ser_buffer = NULL,
             *server_recv_ser_buffer = NULL;

  serlib_serialize_init_buffer_of_size(&server_recv_ser_buffer,
                                 MAX_RECV_SEND_BUFF_SIZE);

  serlib_serialize_init_buffer_of_size(&server_send_ser_buffer,
                                 MAX_RECV_SEND_BUFF_SIZE);

  printf("RPC Server is now listening on port %d...\n", RPC_SERVER_PORT);

  // serlib_reset_buffer(server_recv_ser_buffer);

  return 0;
}

