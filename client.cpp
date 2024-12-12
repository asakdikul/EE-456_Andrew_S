// client.cpp
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // Contains inet_pton
#include <unistd.h>     // Contains close
#include <cstring>      // For strlen
using namespace std;

#define PORT 8080

int main() {
    // Create a socket file descriptor
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation error");
        return -1;
    }
    cout << "Socket created successfully." << endl;

    // Configure server address structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  // Zero out the structure
    serv_addr.sin_family = AF_INET;            // Set IPv4
    serv_addr.sin_port = htons(PORT);          // Convert port to network byte order

    // Convert IPv4 address from text to binary form
    // 127.0.0.1 is localhost - change this to your server's IP if running on different machines
    int addr_result = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if (addr_result == -1) {
        perror("Address conversion error");
        close(sock);
        return -1;
    } else if (addr_result == 0) {
        cout << "Invalid address format" << endl;
        close(sock);
        return -1;
    }

    // Connect to the server
    int connect_result = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect_result == -1) {
        perror("Connection failed");
        close(sock);
        return -1;
    }
    cout << "Connected to the server." << endl;

    // Send message to server
    const char *hello = "Hello from client";
    int send_result = send(sock, hello, strlen(hello), 0);
    if (send_result == -1) {
        perror("Send failed");
        close(sock);
        return -1;
    }
    cout << "Hello message sent." << endl;

    // Receive response from server
    char buffer[1024] = {0};  // Buffer to store server's response
    int read_result = read(sock, buffer, 1024);
    if (read_result == -1) {
        perror("Read failed");
        close(sock);
        return -1;
    }
    cout << "Message from server: " << buffer << endl;

    // Clean up
    close(sock);
    return 0;
}