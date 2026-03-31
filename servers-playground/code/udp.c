#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Bind to port 8080
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];
        
        // Receive data
        int bytes = recvfrom(sock, data, BUFFER_SIZE, 0, 
                            (struct sockaddr*)&client_addr, &client_len);
        
        printf("Received %d bytes from %s:%d\n", 
               bytes,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        
        // Echo back
        sendto(sock, data, bytes, 0, 
              (struct sockaddr*)&client_addr, client_len);
    }
    
    return 0;
}
