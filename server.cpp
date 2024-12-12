// server.cpp
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
using namespace std;

#define PORT 8080

int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    cout << "Socket created successfully." << endl;

    // Configure the server address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Binding successful." << endl;

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    cout << "Listening for connections..." << endl;

    // Accept connection and communicate
    int new_socket;
    socklen_t addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout << "Connection accepted." << endl;

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    cout << "Message from client: " << buffer << endl;

    const char *hello = "Hello from server";
    send(new_socket, hello, strlen(hello), 0);
    cout << "Hello message sent." << endl;

    close(new_socket);
    close(server_fd);
    return 0;
}