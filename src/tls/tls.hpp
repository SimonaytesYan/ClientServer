
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

struct SSLEndpoint {
    SSL *ssl; 
    SSL_CTX *ctx;
};

void initSSL();
void initServerEndpoint(SSLEndpoint* ep, int socket);
void initClientEndpoint(SSLEndpoint* ep, int socket);
