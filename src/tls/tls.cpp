#include "tls.hpp"
#include "../logPrintf.hpp"

const char* kTLSCertificate = "../ca/server.crt";      // "../ca/tls_self_signed_certificate.crt";
const char* kPrivateKey     = "../ca/server.key";      // "../ca/tls_private_key.key";

void initSSL() {
    OPENSSL_init_ssl(0, NULL);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_crypto_strings();
}

void initEndpoint(SSLEndpoint* ep, bool is_server) {
    ep->ctx = SSL_CTX_new(TLS_method());
    if (!ep->ctx) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (is_server) {
        if (SSL_CTX_use_certificate_file(ep->ctx, kTLSCertificate,
                                        SSL_FILETYPE_PEM) != 1) {
            LOG_PRINTF("error SSL_CTX_use_certificate_file");
        }

        if (SSL_CTX_use_PrivateKey_file(ep->ctx, kPrivateKey,
                                    SSL_FILETYPE_PEM)  != 1) {
            LOG_PRINTF("error SSL_CTX_use_PrivateKey_file");
        }

        if (SSL_CTX_check_private_key(ep->ctx) != 1) {
            LOG_PRINTF("private key check");
            perror("private key check");
            exit(1);
        }

        // SSL_CTX_set_keylog_callback(ep->ctx, SLLKeyLogCallback);
    }
    SSL_CTX_set_options(ep->ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3); // Говорим, что любые алгоритмы шифрования подходят 

    ep->ssl = SSL_new(ep->ctx);

    if (is_server)
        SSL_set_accept_state(ep->ssl);
    else
        SSL_set_connect_state(ep->ssl);

  
}