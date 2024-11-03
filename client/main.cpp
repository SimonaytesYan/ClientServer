#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../server/server.hpp"

const char* message = "Hello world!\n";

void udp_client();
void tcp_client();

int main(int argc, char** argv) {

    if (strcmp(argv[1], "tcp")) {
        tcp_client();
    }
    else {
        udp_client();
    }
}

void tcp_client() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = createServerAddr();

    connect(clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);
}

void udp_client() {
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr = createServerAddr();

    send(clientSocket, message, strlen(message), 0);
}