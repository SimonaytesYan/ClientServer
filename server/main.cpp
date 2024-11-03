#include  "server.hpp"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

const size_t kConnectionReqs = 1;
const size_t kBufferSize = 1024;

void tcp_server();
void udp_server();

int main(int argc, char** argv) {

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

    while(true) {
        int clientSocket = accept(server_socket, nullptr, nullptr);
        char buffer[kBufferSize] = {};
        size_t read_n = recv(clientSocket, buffer, sizeof(buffer), 0);

        printf("TCP request (%zu):\n <%s>\n", read_n, buffer);
    }

    close(server_socket);
}

void udp_server() { 
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr = createServerAddr();

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while(true) {
        char buffer[kBufferSize] = {};
        size_t read_n = read(server_socket, buffer, sizeof(buffer));

        printf("UDP request (%zu):\n <%s>\n", read_n, buffer);
    }

    close(server_socket);
}