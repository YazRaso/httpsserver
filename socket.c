#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// see sockets.h for documentation
int create_server_socket(int port_number, const char* ip_address, int max_connection_requests) {
    assert(ip_address);
    assert(port_number >= 0);
    assert(port_number <= 65535);
    // creates stream socket (TCP) protocol on IPv$
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
      perror("Failed to create socket");
      return -1;
    }
    struct sockaddr_in server_address;
    // address family, IPv4 in this case
    server_address.sin_family = AF_INET;
    // convert port_number to unsigned two byte integer
    server_address.sin_port = htons(port_number);
    // convert ip address to be presentable to network
    int presentation_to_network = inet_pton(AF_INET, ip_address, &server_address.sin_addr.s_addr);
    // Enables socket option, SO_REUSEADDR
    int optval = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        // it is considered best practice to first make a shutdown call, before closing the socket
        int shutdown_result = shutdown(socketFD, SHUT_RDWR);
        if (shutdown_result == 0) {
          // close file descriptor
          close(socketFD);
        }
        return -1;
    }
    int bind_result = bind(socketFD, (const struct sockaddr *)(&server_address), sizeof(server_address));
    int listen_result = listen(socketFD, max_connection_requests);
    // Error handling

    if (presentation_to_network == -1) {
      perror("Failed to present to network");
      return -1;
    }
    if (bind_result < 0) {
      perror("Failed to bind socket");
      int shutdown_result = shutdown(socketFD, SHUT_RDWR);
      if (shutdown_result == 0) {
        close(socketFD);
      }
      return -1;
    }
    if (listen_result < 0) {
      perror("Failed to listen");
      int shutdown_result = shutdown(socketFD, SHUT_RDWR);
      if (shutdown_result == 0) {
        close(socketFD);
      }
      return -1;
    }
    return socketFD;
}