#include  "server.hpp"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "../tls/tls.hpp"
#include "../logPrintf.hpp"

const size_t kConnectionReqs = 1;
const size_t kBufferSize     = 1024;
const char*  kEndRequests    = "stop";

void tcp_server(char* key, char* certificate);
void udp_server();
bool work_with_client(TLS& tls, int server_socket);


int main(int argc, char** argv) {

    if (argc < 2) {
        LOG_PRINTF("Wrong numer of arguments\n");
    }

    if (!strcmp(argv[1], "tcp")) {
        LOG_PRINTF("TCP server\n");
        if (argc == 4)
            tcp_server(argv[2], argv[3]);
        else if (argc == 2)
            tcp_server(nullptr, nullptr);
        else
            LOG_PRINTF("Wrong numer of arguments\n");
    }
    else {
        LOG_PRINTF("UDP server\n");
        udp_server();
    }
}

void tcp_server(char* key, char* certificate) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr = createServerAddr();
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, kConnectionReqs);
    
    TLS tls;

    bool end_recv = false;
    while(true) { 
        end_recv = work_with_client(tls, server_socket);
        if (end_recv)
            break;
    }

    close(server_socket);
}

bool work_with_client(TLS& tls, int server_socket) {
    LOG_PRINTF("Work with client\n");

    int client_socket = accept(server_socket, nullptr, nullptr);

    SSLEndpoint ep = tls.getServerEndpoint(client_socket);

    int err = SSL_accept(ep.ssl);
    if (err <= 0) {
        LOG_PRINTF("Error creating SSL connection.  err=%x\n", err);
        return false;
    }

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
            return true;
        }
    
        SSL_write(ep.ssl, buffer, strlen(buffer));
    }
    close(client_socket);

    return false;
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