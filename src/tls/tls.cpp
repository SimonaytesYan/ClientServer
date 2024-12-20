#include "tls.hpp"
#include "../logPrintf.hpp"

static void SLLKeyLogCallback(const SSL* sll, const char* line);

TLS::TLS(const char* certificate, const char* key) :
    TLSCertificate(certificate),
    privateKey(key) {
    OPENSSL_init_ssl(0, NULL);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_crypto_strings();
}

SSLEndpoint TLS::getServerEndpoint(int socket) {
    SSLEndpoint ep;

    ep.ctx = SSL_CTX_new(TLS_method());
    if (!ep.ctx) {
        LOG_PRINTF("Error during creating ctx\n");        
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    
    if (SSL_CTX_use_certificate_file(ep.ctx, TLSCertificate,
                                        SSL_FILETYPE_PEM) != 1) {
            LOG_PRINTF("error SSL_CTX_use_certificate_file");
    }

    if (SSL_CTX_use_PrivateKey_file(ep.ctx, privateKey,
                                    SSL_FILETYPE_PEM)  != 1) {
        LOG_PRINTF("error SSL_CTX_use_PrivateKey_file");
    }

    if (SSL_CTX_check_private_key(ep.ctx) != 1) {
        LOG_PRINTF("private key check");
        perror("private key check");
        exit(1);
    }

    SSL_CTX_set_keylog_callback(ep.ctx, SLLKeyLogCallback);
    SSL_CTX_set_options(ep.ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);


    ep.ssl = SSL_new(ep.ctx);
    SSL_set_accept_state(ep.ssl);
    SSL_set_fd(ep.ssl, socket);

    LOG_PRINTF("getServerEndpoint successfully\n");

    return ep;
}

SSLEndpoint TLS::getClientEndpoint(int socket) {
    SSLEndpoint ep;
    
    ep.ctx = SSL_CTX_new(TLS_method());
    if (!ep.ctx) {
        LOG_PRINTF("Error during creating ctx\n");        
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    SSL_CTX_set_options(ep.ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);

    ep.ssl = SSL_new(ep.ctx);
    SSL_set_connect_state(ep.ssl);
    SSL_set_fd(ep.ssl, socket);

    LOG_PRINTF("getClientEndpoint successfully\n");

    return ep;
}

static void SLLKeyLogCallback(const SSL* sll, const char* line) {
    char* env_p = getenv("SSLKEYLOGFILE");
    printf("env_p = %s\n", env_p);

    FILE* fp = nullptr;
    if (env_p) {
        if (fp = fopen(env_p, "a+")) {
            fprintf(fp, "%s\n", line);
            fclose(fp);
        }
    }
}
