#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "kerberos.h"
#include "tcp.h"

static void
usage(const char *name)
{
    fprintf(stderr, "Usage: %s [-d daemon] [-p port] [-v verbose] [-V version]\n",
            name);
}

static void
version(const char *version)
{
    printf("kerberos %s\n \
            \rCopyright (c) 2022 Vinícius Kury\n \
            \rMIT License <https://mit.org>\n \
            \rThis is free software: you are free to change and redistribute it.\n \
            \rThere is NO WARRANTY, to the extent permitted by law\n\n \
            \rWritten by Vinícius Kury\n",
            version);
}

int
main(int argc, char *argv[])
{
    in_addr_t addr = INADDR_ANY;
    in_port_t port = KERBEROS_PORT;
    struct sockaddr_in address;
    socklen_t address_len = sizeof address;
    int client_fd, opt, server_fd, sock;
    int as, is_daemon, is_verbose, tgs;

    while ((opt = getopt(argc, argv, "adhp:stvV")) != -1) {
        switch (opt) {
        case 'a':
            as = 1;
            break;
        case 'd':
            is_daemon = 1;
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 'p':
            if (atoi(optarg) > USHRT_MAX) ERROR("Port value out of range");
            port = atoi(optarg);
            break;
        case 't':
            tgs = 1;
            break;
        case 'v':
            is_verbose = 1;
            break;
        case 'V':
            version(VERSION);
            exit(EXIT_SUCCESS);
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
    if (is_daemon == 1) {
        #ifdef DEBUG
            if(daemon(1, 1)) ERROR(strerror(errno));
        #else
            if(daemon(1, 0)) ERROR(strerror(errno));
        #endif

        server_fd = tcp_sock_server(address);

        printf("Started server at address %s port %d\n",
                inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        while (1) {
            sock = accept(server_fd, (struct sockaddr *)&address, &address_len);
            if (sock == -1) ERROR(strerror(errno));

            printf("Accepted connection at address %s port %d\n",
                    inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            if (close(sock)) ERROR(strerror(errno));
        }

        if (shutdown(server_fd, SHUT_RDWR)) ERROR(strerror(errno));

    } else {
        /* run as client */
        client_fd = tcp_sock_client();
        if (connect(client_fd, (struct sockaddr *)&address, address_len))
            ERROR(strerror(errno));
        if (is_verbose == 1) {
            printf("Connected to server at address %s port %d\n",
                    inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        }
        if (as == 1) {
            send(client_fd, "a", 2, 0);
        } else if (tgs == 1) {
            send(client_fd, "t", 2, 0);
        } else {
            if (close(client_fd)) ERROR(strerror(errno));
        }
    }

    return 0;
}
