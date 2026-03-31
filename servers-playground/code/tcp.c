#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int b = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    int list = listen(sock, 5);

    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];

        int client_sock = accept(sock, (struct sockaddr*) &client_addr, &client_len);

        int bytes = recv(client_sock, data, 1024, 0);
        printf("receiving bytes: %d\n", bytes);
        printf("receiving data: %s\n", data);
        // send(client_sock, data, bytes, 0);
        close(client_sock);
    }
}
