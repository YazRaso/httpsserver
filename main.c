#include <openssl/err.h>
#include <openssl/ssl.h>

// Call SSL_load_error_strings() and OpenSSL_add_ssl_algorithms() at startup
// Call EVP_cleanup() to free memory