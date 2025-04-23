#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// see sockets.h for documentation
int create_socket(int port_number, char* ip_address) {
    assert(ip_address);
    assert(port_number >= 0);
    assert(port_number <= 65535);
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    inet_pton(AF_INET, ip_address, &server_address.sin_addr.s_addr);
    int result = connect(socketFD, &server_address, sizeof(server_address));
    return result;
}