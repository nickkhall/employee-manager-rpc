#ifndef __EMP_MAN_REST_SOCKETS__
#define __EMP_MAN_REST_SOCKETS__

#include <sys/socket.h>

/*
 * -------------------------------------------
 *
 * -------------------------------------------
 *
 * -------------------------------------------
 *
 * -------------------------------------------
 */
int* socklib_socket_create();


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
struct sockaddr_in* socklib_socket_build_sock_addr_in(int* sockfd, int family, int port);

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
                             socklen_t addr_len);

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
                             socklen_t* addr_len);

#endif

