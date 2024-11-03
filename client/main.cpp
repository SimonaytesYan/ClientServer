#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    const char* message = "Hello world!\n";
    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);
}