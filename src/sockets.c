#include <memory.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers/sockets.h"
#include "headers/common.h"

/*
 * -------------------------------------------
 *
 * -------------------------------------------
 *
 * -------------------------------------------
 *
 * -------------------------------------------
 */
int* socklib_socket_create() {
  // destination socket pointer
  struct sockaddr_in* dest = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  int addr_family = AF_INET;
  int addr_port = htons(RPC_SERVER_PORT);

  // socket variables
  int* sockfd = (int*) malloc(sizeof(int));

  //dest->sin_family = AF_INET;
  //dest->sin_port = htons(RPC_SERVER_PORT);
  memcpy(&dest->sin_family, &addr_family, sizeof(int));
  memcpy(&dest->sin_port, &addr_port, sizeof(int));

  struct hostent* host = (struct hostent*) gethostbyname("127.0.0.1");
  
  // dest.sin_addr = *((struct in_addr*) host->h_addr_list);
  memcpy(&dest->sin_addr, host->h_addr_list[0], host->h_length);

  int socket_type = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  *sockfd = socket_type;
  if (*sockfd == -1) {
    printf("ERROR:: Socket creation failed...\n");
    return NULL;
  }

  return sockfd;
};

/*
 * ---------------------------------------------------
 * function: socklib_socket_build_sock_addr_in
 * ---------------------------------------------------
 * params  :
 *         > family - int
 *         > port   - int
 * ---------------------------------------------------
 * Handles building a sock_addr struct.
 * ---------------------------------------------------
 */
struct sockaddr_in* socklib_socket_build_sock_addr_in(int* sockfd, int family, int port)
{
  struct sockaddr_in* dest = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  if (!dest) {
    printf("ERROR:: REST - Failed to allocate memory for socket addres in build_sock_addr_in\n");
    return NULL;
  }

  dest->sin_family = family;
  dest->sin_port = htons(port);

  struct hostent* host = (struct hostent*) gethostbyname("127.0.0.1");
  if (!host) {
    printf("ERROR:: REST - Failed to get host in build_sock_addr_in\n");
    return NULL;
  }

  memcpy(&dest->sin_addr, host->h_addr_list[0], host->h_length);

  return dest;
};

/*
 * ---------------------------------------------------
 * function: socklib_socket_send_to
 * ---------------------------------------------------
 * params  :
 *         > sockfd           - int*
 *         > buffer           - char*
 *         > buffer_data_size - size_t
 *         > flags            - int
 *         > dest_addr        - struct sockaddr*
 *         > addr_len         - socklen_t*
 * ---------------------------------------------------
 * Handles sending data to socket.
 * ---------------------------------------------------
 */
int socklib_socket_send_to(int* sockfd, const void* buffer,
                             int buffer_data_size,
                             int flags,
                             const struct sockaddr* dest_addr,
                             socklen_t addr_len)
{
  int res = sendto(*sockfd, (char*)buffer,
                buffer_data_size,
                flags,
                dest_addr,
                addr_len);

  if (res == -1) {
    printf("ERROR:: REST - Failed to send data to rpc,\n sockfd: %d,\n buffer: %s,\n buffer_data_size: %d,\nflags: %d,\naddr_len: %d\n",
            *sockfd, (char*)buffer, buffer_data_size, flags, addr_len);
    free(sockfd);
    free((char*)buffer);
    // for now, just terminate application
    exit(1);
  }

  return res;
};

/*
 * ---------------------------------------------------
 * function: socklib_socket_recv_from
 * ---------------------------------------------------
 * params  :
 *         > sockfd        - int*
 *         > buffer        - char*
 *         > buffer_length - size_t
 *         > flags         - int
 *         > src_addr      - struct sockaddr*
 *         > addr_len      - socklen_t*
 * ---------------------------------------------------
 * Handles receiving data from socket.
 * ---------------------------------------------------
 */
int socklib_socket_recv_from(int* sockfd, char* buffer,
                             size_t buffer_length,
                             int flags,
                             struct sockaddr* src_addr,
                             socklen_t* addr_len)
{
  return recvfrom(*sockfd, buffer, buffer_length, flags, src_addr, addr_len);
};

