#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/ssl.h>
void ERR_load_crypto_strings(void);
void ERR_free_strings(void);
void SSL_load_error_strings(void);
int SSL_library_init(void);
#define OpenSSL_add_ssl_algorithms()    SSL_library_init()

// Call EVP_cleanup() to free memory