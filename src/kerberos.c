#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "kerberos.h"
#include "tcp.h"

#define KERBEROS_PORT 8080

static void
usage(const char *name)
{
    fprintf(stderr, "Usage: %s [-d daemon] [-p port]\n", name);
}

int
main(int argc, char *argv[])
{
    in_addr_t addr = INADDR_ANY;
    in_port_t port = KERBEROS_PORT;
    struct sockaddr_in address;
    socklen_t address_len = sizeof address;
    int client_fd, is_daemon = 0, opt, server_fd, sock;

    while ((opt = getopt(argc, argv, "dp:s")) != -1) {
        switch (opt) {
        case 'd':
            is_daemon = 1;
            break;
        case 'p':
            if (atoi(optarg) > USHRT_MAX) ERROR("Port value out of range");
            port = atoi(optarg);
            break;
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = addr;
    address.sin_port = htons(port);

    /* daemonize and create server socket */
    if (is_daemon) {
        #ifdef DEBUG
            if(daemon(1, 1)) ERROR(strerror(errno));
        #else
            if(daemon(1, 0)) ERROR(strerror(errno));
        #endif

        server_fd = tcp_sock_server(address);

        #ifdef DEBUG
            printf("Started server at port %d\n", port);
        #endif

        while (1) {
            sock = accept(server_fd, (struct sockaddr *)&address, &address_len);
            if (sock) ERROR(strerror(errno));
            #ifdef DEBUG
                printf("Accepted Connection\n");
            #endif
            if (close(sock)) ERROR(strerror(errno));
        }

        if (shutdown(server_fd, SHUT_RDWR)) ERROR(strerror(errno));

    } else {
        /* run as client */
        client_fd = tcp_sock_client();
        if (connect(client_fd, (struct sockaddr *)&address, address_len))
            ERROR(strerror(errno));
        #ifdef DEBUG
            printf("Connected to server\n");
        #endif
    }

    return 0;
}
