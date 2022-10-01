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
    fprintf(stderr, "Usage: %s [-adhpvV]\n",
            name);
}

static void
version(const char *version)
{
    printf("kerberos %s\n \
            \rCopyright (c) 2022 Vinícius Kury\n \
            \rMIT License <https://mit-license.org>\n \
            \rThis is free software: you are free to change and redistribute it.\n \
            \rThere is NO WARRANTY, to the extent permitted by law\n\n \
            \rWritten by Vinícius Kury\n",
            version);
}

int
main(int argc, char *argv[])
{
    struct in_addr addr;
    addr.s_addr = INADDR_ANY;
    in_port_t port = KERBEROS_PORT;
    struct sockaddr_in address;
    int client_fd, is_verbose, opt;
    static char buf[BUFSIZ];

    while ((opt = getopt(argc, argv, "ad:hp:tvV")) != -1) {
        switch (opt) {
        case 'a':
            buf[0] = 'a';
            break;
        case 'd':
            if(!inet_aton(optarg, &addr)) ERROR("Invalid IP address");
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
            buf[0] = 't';
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
    address.sin_addr.s_addr = addr.s_addr;
    address.sin_port = htons(port);

    client_fd = tcp_sock_client();
    if (connect(client_fd, (struct sockaddr *)&address, sizeof address))
        ERROR(strerror(errno));
    if (is_verbose == 1) {
        printf("Connected to server at address %s port %d\n",
                inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    }
    send(client_fd, buf, sizeof buf, 0);

    if (close(client_fd)) ERROR(strerror(errno));

    return 0;
}
