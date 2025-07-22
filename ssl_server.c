#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"
#include <string.h>
#define PORT 4443

int main(void) {
    // Set up SSL_CTX ONCE
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (!ctx) { perror("Failed to create SSL context"); exit(EXIT_FAILURE); }
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        perror("Failed to load certificate or key");
        exit(EXIT_FAILURE);
    }

    // Create server socket ONCE
    int server_fd = create_server_socket(PORT, "127.0.0.1", 10);

    // Infinite loop to handle clients
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("Failed to accept connection"); continue; }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0) {
            SSL_free(ssl);
            close(client_fd);
            continue;
        }

        char buffer[8192];
        int total_received = 0;
        while (1) {
            int r = SSL_read(ssl, buffer + total_received, sizeof(buffer) - total_received - 1);
            if (r <= 0) break;
            total_received += r;
            buffer[total_received] = '\0';
            if (strstr(buffer, "\r\n\r\n")) break; // End of headers
        }

        printf("Received request:\n%s\n", buffer);

        if (strncmp(buffer, "GET ", 4) == 0) {
            char *file_path = buffer + 4;
            char *end_of_request = strchr(file_path, ' ');
            if (end_of_request) {
                *end_of_request = '\0';
            }
            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from SSL server!";
            SSL_write(ssl, response, strlen(response));
        } else if (strncmp(buffer, "POST ", 5) == 0) {
            printf("Handling POST request...\n");

            // Find Content-Length
            char *content_length_str = strstr(buffer, "Content-Length:");
            int content_length = 0;
            if (content_length_str) {
                sscanf(content_length_str, "Content-Length: %d", &content_length);
                printf("Content-Length = %d\n", content_length);
            } else {
                printf("No Content-Length found, cannot read body.\n");
            }

            // Find where the body starts (after \r\n\r\n)
            char *body = strstr(buffer, "\r\n\r\n");
            if (body) {
                body += 4; // Skip over the \r\n\r\n
            } else {
                printf("No body found.\n");
            }

            int body_bytes_already_read = total_received - (body - buffer);
            int remaining_bytes = content_length - body_bytes_already_read;

            while (remaining_bytes > 0) {
                int r = SSL_read(ssl, body + body_bytes_already_read, remaining_bytes);
                if (r <= 0) {
                    printf("Failed to read full POST body.\n");
                    break;
                }
                body_bytes_already_read += r;
                remaining_bytes -= r;
            }
            printf("POST body received:\n%.*s\n", content_length, body);
            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST received.";
            SSL_write(ssl, response, strlen(response));
        } else {
            const char *response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\nUnsupported HTTP method.";
            SSL_write(ssl, response, strlen(response));
            printf("Unsupported HTTP method received:\n%s\n", buffer);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
    }

    close(server_fd);
    SSL_CTX_free(ctx);
}
