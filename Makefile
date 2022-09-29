# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

BIN = kerberos
CPPFLAGS = -DDEBUG
CFLAGS = -g -pipe -Wall -Wextra -Wformat=2 -Wpedantic -Wshadow
LDFLAGS =

BUILD_DIR = build
SRC_DIR = src

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: clean
