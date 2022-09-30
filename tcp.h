#ifndef _TCP_H
#define _TCP_H

#include <netinet/in.h>

/**
 * @brief Create a TCP client socket and return its fd.
 * 
 * @return the client socket file descriptor
 */
int tcp_sock_client(void);

/**
 * @brief Listen for TCP connections at address
 * 
 * @param address the address struct containing the ip address and port
 * @return the file descriptor of the server socket
 */
int tcp_sock_server(const struct sockaddr_in address);

#endif
