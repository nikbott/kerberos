#ifndef _KERBEROS_H
#define _KERBEROS_H

#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.1"
#define ERROR(X) { fprintf(stderr, "Error: %s\n", X); exit(EXIT_FAILURE); }
#define KERBEROS_PORT 8080

int
authentication_server(void);

int
ticket_granting_server(void);

#endif
