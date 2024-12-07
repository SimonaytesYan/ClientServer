#include  "server.hpp"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../tls/tls.hpp"
#include "../logPrintf.hpp"

const size_t kConnectionReqs = 1;
const size_t kBufferSize     = 1024;
const char*  kEndRequests    = "stop";

void tcp_server();
void udp_server();

int main(int argc, char** argv) {

    if (argc != 2) {
        LOG_PRINTF("Wrong numer of arguments\n");
    }

    if (!strcmp(argv[1], "tcp")) {
        LOG_PRINTF("TCP server\n");
        tcp_server();
    }
    else {
        LOG_PRINTF("UDP server\n");
        udp_server();
    }
}

void tcp_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr = createServerAddr();
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, kConnectionReqs);

    initSSL();

    bool end_recv = false;
    while(true) { 

        int client_socket = accept(server_socket, nullptr, nullptr);

        SSLEndpoint ep;
        initServerEndpoint(&ep, client_socket);

        int err = SSL_accept(ep.ssl);
        if (err <= 0) {
            LOG_PRINTF("Error creating SSL connection.  err=%x\n", err);
            return;
        }
        LOG_PRINTF("SSL_accept successfully!\n");

        while (true) {
            char buffer[kBufferSize] = {};
            
            int read_n = SSL_read(ep.ssl, buffer, sizeof(buffer));
            if (read_n == 0) {
                LOG_PRINTF("server: stop reading\n");
                break;
            }

            LOG_PRINTF("server: TCP (%d) <%s>\n", read_n, buffer);
            memset(buffer, 0, kBufferSize);

            scanf("%s", buffer);
            if (!strcmp(buffer, kEndRequests)) {
                end_recv = true;
                break;
            }
    
            SSL_write(ep.ssl, buffer, strlen(buffer));
        }
        close(client_socket);

        if (end_recv)
            break;
    }

    close(server_socket);
}

void udp_server() { 
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr     = createServerAddr();
    sockaddr_in client_addr     = {};
    socklen_t   client_addr_len = sizeof(client_addr);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while(true) {
        char buffer[kBufferSize] = {};
        memset(buffer, 0, kBufferSize);

        size_t read_n = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                                 (sockaddr*)&client_addr, &client_addr_len);

        LOG_PRINTF("server: UDP (%d) <%s>\n", read_n, buffer);
        memset(buffer, 0, kBufferSize);

        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        sendto(server_socket, buffer, strlen(buffer), 0, 
               (sockaddr*)&client_addr, client_addr_len);
    }

    close(server_socket);
}