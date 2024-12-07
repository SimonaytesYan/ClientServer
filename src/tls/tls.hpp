
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

struct SSLEndpoint {
    SSL *ssl; 
    SSL_CTX *ctx;
};

struct TLS {
    TLS(const char* certificate = "../ca/server.crt", const char* key = "../ca/server.key");

    SSLEndpoint getClientEndpoint(int socket);
    SSLEndpoint getServerEndpoint(int socket);

private:
    const char* TLSCertificate = "../ca/server.crt";
    const char* privateKey     = "../ca/server.key";
};
