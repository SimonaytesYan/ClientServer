#include  "server.hpp"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

const size_t kConnectionReqs = 1;
const size_t kBufferSize     = 1024;
const char*  kEndRequests    = "stop";

void tcp_server();
void udp_server();

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Wrong numer of arguments\n");
    }

    if (!strcmp(argv[1], "tcp")) {
        printf("TCP server\n");
        tcp_server();
    }
    else {
        printf("UDP server\n");
        udp_server();
    }
}

void tcp_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = createServerAddr();

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, kConnectionReqs);

    bool end_recv = false;
    while(true) { 
        int client_socket = accept(server_socket, nullptr, nullptr);

        while (true) {
            char buffer[kBufferSize] = {};

            ssize_t read_n = recv(client_socket, buffer, sizeof(buffer), 0);
            if (read_n == 0) {
                printf("server: stop reading\n");
                break;
            }

            printf("server: TCP (%zu) <%s>\n", read_n, buffer);
            memset(buffer, 0, kBufferSize);

            scanf("%s", buffer);
            if (!strcmp(buffer, kEndRequests)) {
                end_recv = true;
                break;
            }

            send(client_socket, buffer, strlen(buffer), 0);
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

        printf("server: UDP (%zu) <%s>\n", read_n, buffer);
        memset(buffer, 0, kBufferSize);

        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        sendto(server_socket, buffer, strlen(buffer), 0, 
               (sockaddr*)&client_addr, client_addr_len);
    }

    close(server_socket);
}