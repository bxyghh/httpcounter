#include "TcpServer.h"

#include <sys/socket.h> // For socket(), bind(), listen(), accept()
#include <netinet/in.h> // For sockaddr_in, INADDR_ANY
#include <arpa/inet.h> // For inet_addr()
#include <unistd.h> // For close()
#include <cstdio> // For perror()
#include <cstdlib> // For atoi()

int TcpServer::ListenAndServe(const char *host, short port, int backlog)
{
    // Create socket
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1) {
        perror("Failed to create socket");
        return -1;
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("Failed to set socket options");
        return -1;
    }
    
    // Bind socket to address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host);
    address.sin_port = htons(port);
    
    if (bind(listenSocket, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Failed to bind socket");
        return -1;
    }
    
    // Listen for incoming connections
    if (listen(listenSocket, backlog) == -1) {
        perror("Failed to listen for connections");
        return -1;
    }
    
    // Accept and handle connections
    while (true) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        
        int clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            perror("Failed to accept connection");
            return -1;
        }
        
        // Call the handler function to process the connection
        _handler->ServeNewConnection(clientSocket);
    }
    
    // Close the listen socket
    close(listenSocket);
    
    return 0;
}