#ifndef _KERBEROS_H
#define _KERBEROS_H

#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.1"
#define ERROR(X) { fprintf(stderr, "Error: %s\n", X); exit(EXIT_FAILURE); }
#define KERBEROS_PORT ((in_port_t) 0x1f90)

int
as(void);

int
tgs(void);

#endif
