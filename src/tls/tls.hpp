
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

struct SSLEndpoint {
    SSL *ssl; 
    SSL_CTX *ctx;
};

void initSSL();

void initEndpoint(SSLEndpoint* ep, bool is_server);
