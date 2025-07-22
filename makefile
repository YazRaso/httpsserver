CC = gcc
# NB: The file path for OpenSSL may be different for you!
CFLAGS = -Wall -Wextra -g -I$(shell brew --prefix openssl@3)/include
LDFLAGS = -L$(shell brew --prefix openssl@3)/lib -lssl -lcrypto

SRCS = ssl_server.c socket.c
OBJS = $(SRCS:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
