#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>

int main (void) {
    // Set up SSL

    SSL_CTX *ctx = SSL_CTX_new(TLS_method());

    if (ctx) {
        int cert = SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM);
        int private_key = SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM);
    } else {
        printf("Failed to initialize SSL! Exiting");
        exit(1);
    }
}