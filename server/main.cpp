#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

const size_t kConnectionReqs = 1;
const size_t kBufferSize = 1024;

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, kConnectionReqs);

    
    while(true) {
        int clientSocket = accept(server_socket, nullptr, nullptr);
        char buffer[kBufferSize] = {};
        size_t read_n = recv(clientSocket, buffer, sizeof(buffer), 0);

        printf("request (%zu):\n <%s>\n", read_n, buffer);
    }

    close(server_socket);
}