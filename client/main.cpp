#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../server/server.hpp"

const char* message = "Hello world!";

const size_t kBufferSize = 1024;
const char* kEndRequests = "stop";

void udp_client();
void tcp_client();

int main(int argc, char** argv) {
    if (!strcmp(argv[1], "tcp")) {
        printf("TCP client\n");
        tcp_client();
    }
    else {
        printf("UDP client\n");
        udp_client();
    }
}

void tcp_client() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = createServerAddr();

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (true) {
        char buffer[kBufferSize] = {};

        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        send(client_socket, buffer, strlen(buffer), 0);
    
        memset(buffer, 0, kBufferSize);
        size_t read_n = recv(client_socket, buffer, sizeof(buffer), 0);
        printf("client: TCP (%zu)<%s>\n", read_n, buffer);
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
        printf("client: UDP (%zu)<%s>\n", read_n, buffer);
    }

    close(client_socket);
}