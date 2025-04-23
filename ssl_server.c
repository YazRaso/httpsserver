#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include "socket.h"
#define PORT 4443
int main (void) {
    // Set up SSL
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx == NULL) {
        perror("Failed to create SSL context");
        exit(EXIT_FAILURE);
    }
    int cert = SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM);
    int private_key = SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM);

    // Setup server socket
    const char *ip_address = "127.0.0.1"; // Local host
    int number_of_connections = 10;
    int server_fd = create_server_socket(PORT, ip_address, number_of_connections);
    struct sockaddr_in client_addr;

    // Accept requests
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, &client_addr, &client_len);
    if (client_fd < 0) {
        perror("Failed to accept connection");
        exit(EXIT_FAILURE);
    }

    // Create ssl connection
    SSL *ssl = SSL_new(ctx);
    // While we have multiple ssl connections, we only have one ssl context for the whole server
    if (ssl == NULL) {
        perror("Failed to create SSL object");
        exit(EXIT_FAILURE);
    }
    // Start TLS handshake
    SSL_set_fd(ssl, client_fd);
    int handshake_result = SSL_accept(ssl);
    if (handshake_result < 0) {
        int err = SSL_get_error(ssl, handshake_result);
        fprintf(stderr, "SSL handshake failed: %d\n", err);
        exit(EXIT_FAILURE);
    }


    // Print client information
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Free up and close allocated memory and file descriptors
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
    close(server_fd);
    SSL_CTX_free(ctx);
}