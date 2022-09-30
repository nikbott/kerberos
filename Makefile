# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

CPPFLAGS = -DDEBUG
CFLAGS = -g -pipe -Wall -Wextra -Wformat=2 -Wpedantic -Wshadow
LDFLAGS =

KERBEROS_SRC = kerberos.c tcp.c
KERBEROSD_SRC = as.c kerberosd.c tcp.c tgs.c

all: kerberos kerberosd

kerberos: $(KERBEROS_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

kerberosd: $(KERBEROSD_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) kerberos kerberosd

.PHONY: clean
