#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../server/server.hpp"
#include "../tls/tls.hpp"
#include "../logPrintf.hpp"

const char* message = "Hello world!";

const size_t kBufferSize = 1024;
const char* kEndRequests = "stop";

void udp_client();
void tcp_client();

int main(int argc, char** argv) {

    if (argc != 2) {
        LOG_PRINTF("Wrong numer of arguments\n");
    }

    if (!strcmp(argv[1], "tcp")) {
        LOG_PRINTF("TCP client\n");
        tcp_client();
    }
    else {
        LOG_PRINTF("UDP client\n");
        udp_client();
    }
}

void tcp_client() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr = createServerAddr();
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    initSSL();

    SSLEndpoint ep;
    initClientEndpoint(&ep, client_socket);

    int err = SSL_connect(ep.ssl);
    if (err <= 0) {
        LOG_PRINTF("Error creating SSL connection.  err=%x\n", err);
        return;
    }

    LOG_PRINTF("SSL_connect successfully!\n");

    while (true) {
        char buffer[kBufferSize] = {};

        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        int res = SSL_write(ep.ssl, buffer, strlen(buffer));
    
        memset(buffer, 0, kBufferSize);
        int read_n = SSL_read(ep.ssl, buffer, sizeof(buffer));
        LOG_PRINTF("client: TCP (%d)<%s>\n", read_n, buffer);
    }

    close(client_socket);
}

void udp_client() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr     = createServerAddr();
    socklen_t   server_addr_len = sizeof(server_addr);
    while (true) {
        char buffer[kBufferSize] = {};
        
        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        sendto(client_socket, buffer, strlen(buffer), 0, 
               (sockaddr*)&server_addr, server_addr_len);
        
        memset(buffer, 0, kBufferSize);
        size_t read_n = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                 nullptr, nullptr);
        LOG_PRINTF("client: UDP (%d)<%s>\n", read_n, buffer);
    }

    close(client_socket);
}