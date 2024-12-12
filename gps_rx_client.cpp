#include <iostream>
#include <RadioLib.h>
#include "PiHal.h"
#include <cstdio>

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // Contains inet_pton
#include <unistd.h>     // Contains close
#include <cstring>      // For strlen
using namespace std;

#define PORT 8080

// Create a new instance of the HAL class
PiHal* hal = new PiHal(0); // 0 for SPI 0 , set to 1 if using SPI 1(this will change NSS pinout)

// Create the radio module instance/////////////////////////
// Pinout *****MBED SHIELD****************PI HAT************
// NSS pin:  WPI# 10 (GPIO 8)  WPI # 29 (GPIO 21) for Pi hat
// DIO1 pin: WPI# 2  (GPIO 27) WPI # 27 (GPIO 16) for Pi hat
// NRST pin: WPI# 21 (GPIO 5)  WPI # 1  (GPIO 18) for Pi hat
// BUSY pin: WPI# 0  (GPIO 17) WPI # 28 (GPIO 20) for Pi hat
////////////////////////////////////////////////////////////

// Radio initialization based on Pi Hat wiring
// change for MBED Shield use
// According to SX1262 radio = new Module(hal, NSS,DI01,NRST,BUSY)
Module* module = new Module(hal, 29, 27, 1, 28);
SX1262 radio(module);

char* gps_message;

// Flag to indicate packet has been received
volatile bool receivedFlag = false;

// ISR function to set received flag
void setFlag(void) {
  receivedFlag = true;
}

int main(int argc, char** argv) {
  hal->init(); 
  radio.XTAL = true;

  // Initialize the radio module
  printf("[SX1262] Initializing ... ");
  int state = radio.begin(915.0, 125.0, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 0.0, false);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return(1);
  }
  printf("success!\n");

  // Set the ISR for packet received
  radio.setPacketReceivedAction(setFlag);

  // Start receiving packets
  printf("[SX1262] Starting to listen ... ");
  state = radio.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return(1);
  }
  printf("success!\n");

  // Main loop
  uint8_t buff[256] = { 0 };
  for(;;) {
    if(receivedFlag) {
      // Reset flag
      receivedFlag = false;
      
      // Get the length of the received packet
      size_t len = radio.getPacketLength();
      
      // Read the packet
      int state = radio.readData(buff, len);
      if (state != RADIOLIB_ERR_NONE) {
        printf("Read failed, code %d\n", state);
      } else {
        gps_message = (char*)buff;
        break;
      }
    }
  }

  // Create a socket file descriptor
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
      perror("Socket creation error");
      return -1;
  }
  cout << "Socket created successfully." << endl;

  // Configure server address structure
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); // Zero out the structure
  serv_addr.sin_family = AF_INET;           // Set IPv4
  serv_addr.sin_port = htons(PORT);         // Convert port to network byte order

  // Convert IPv4 address from text to binary form
  // 127.0.0.1 is localhost - change this to your server's IP if running on different machines
  int addr_result = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  if (addr_result == -1)
  {
      perror("Address conversion error");
      close(sock);
      return -1;
  }
  else if (addr_result == 0)
  {
      cout << "Invalid address format" << endl;
      close(sock);
      return -1;
  }

  // Connect to the server
  int connect_result = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (connect_result == -1)
  {
      perror("Connection failed");
      close(sock);
      return -1;
  }
  cout << "Connected to the server." << endl;

  // Send message to server
  const char *hello = gps_message;
  int send_result = send(sock, hello, strlen(hello), 0);
  if (send_result == -1)
  {
      perror("Send failed");
      close(sock);
      return -1;
  }
  cout << "Hello message sent." << endl;

  // Receive response from server
  char buffer[1024] = {0}; // Buffer to store server's response
  int read_result = read(sock, buffer, 1024);
  if (read_result == -1)
  {
      perror("Read failed");
      close(sock);
      return -1;
  }
  cout << "Message from server: " << buffer << endl;

  // Clean up
  close(sock);

  return(0);
}