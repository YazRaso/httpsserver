CC = gcc
# NB: The file path for OpenSSL may be different for you!
CFLAGS = -Wall -Wextra -g -I/opt/homebrew/Cellar/openssl@3/3.4.1/include
LDFLAGS = -L/opt/homebrew/Cellar/openssl@3/3.4.1/lib -lssl -lcrypto

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
