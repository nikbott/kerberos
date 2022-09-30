#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include "kerberos.h"
#include "tcp.h"

int
tcp_sock_client(void)
{
    int client_fd;

    /* create tcp socket and return its fd */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) ERROR(strerror(errno));

    return client_fd;
}

int
tcp_sock_server(const struct sockaddr_in address)
{
    int server_fd;
    const int opt = 1;

    /* create tcp socket and return its fd */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) ERROR(strerror(errno));

    /* let socket reuse address and port as needed */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt))
        ERROR(strerror(errno));

    /* bind socket to specified address and port */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof address))
        ERROR(strerror(errno));

    /* prepare socket to accept incoming connections */
    if (listen(server_fd, SOMAXCONN)) ERROR(strerror(errno));

    return server_fd;
}
