# EE-456_Andrew_S

# ADAPTED FROM BEN DUVAL'S INSTRUCTIONS
# EE456  
# LoRa 1262 Project  

**Class:** EE456  
**Project Name:** LoRa 1262  

This project utilizes the SX1262 chip and Raspberry Pi to transmit and receive data using LoRa and FSK modulation.

***********VERY IMPORTANT***********  
- Enable SPI before attempting to transmit or receive.  
Run the following commands:  
`sudo raspi-config`  
Navigate to `Interfacing Options` -> `SPI` and enable it.  
Reboot your Raspberry Pi.  
**********************************************************  

## Installation Instructions  

--Run  `sudo apt update && sudo apt install -y cmake git`  

1. Clone the GitHub Repository for EE456:  
`git clone https://github.com/BenDuval/EE456.git`  
Navigate into the 'EE456' directory.  

2. Navigate to 'WiringPi' directory:  
Run:  
`./build`  
  

3. Navigate to 'RadioLib/examples/NonAudrino/Raspberry/'  
Run:  
`mkdir build`  
`cd build`  
`cmake ..`  
`make`    

4. Make and build the Project:  
From EE456 directory run:  
`mkdir build`  
Navigate to the `build` directory:    
`cd build`  
Run:  
`cmake ../src`  **(cmake must be ran after any changes to CMakeLists.txt such as adding a new executable!)**  
Then run:  
`make` **(make must be ran after all changes to .cpp files)**  

## Wiring Diagram for WiringPi Library & SX1262 MBED Shield  

- NSS: GPIO 8, WPI# 10  
- Reset: GPIO 5, WPI# 21  
- DI01: GPIO 27, WPI# 2  
- Busy: GPIO 17, WPI# 0  
- Pwr: Pin 1 3.3V   
- Gnd: Pick one   
  
Ensure your wiring matches the above configuration for proper operation with the RPI and MBED shield.  

## Wiring Diagram for WiringPi Library & SX1262 LORAWAN Pi Hat  

- NSS: GPIO 21, WPI# 29  
- Reset: GPIO 18, WPI# 1  
- DI01: GPIO 16, WPI# 27  
- Busy: GPIO 20, WPI# 28  

Ensure your wiring matches the above configuration for proper operation with the RPI and SX1262 LORAWAN Pi Hat.  

## Running the Examples  

1. **Transmit Example**:  
Run the `lora_tx.cpp` example to confirm transmission without errors using:  
`sudo ./lora_tx`  

2. **Receive Example**:  
Run the `lora_rx.cpp` example to confirm initialization without any errors using:  
`sudo ./lora_rx`  
Check for errors and verify successful reception.

## Additional Resources  

- [RadioLib Documentation](https://jgromes.github.io/RadioLib/index.html)  
- [RadioLib GitHub Repository](https://github.com/jgromes/RadioLib)  
- [WiringPi Pinout](https://pinout.xyz/pinout/wiringpi)  
- [WiringPi GitHub Repository](https://github.com/WiringPi/WiringPi)  
- [Waveshare SX1262 LoRaWAN Hat](https://www.waveshare.com/sx1262-lorawan-hat.htm)  
- [Semtech SX1262 Product Page](https://www.semtech.com/products/wireless-rf/lora-connect/sx1262)  

# END OF ADAPTATIONS, DIRECTIONS ON HOW TO RUN GPS TO CLIENT, AND CLIENT TO SERVER SOCKET PROGRAMMING

3a. ** GPS Transmit to LoRa Receive Client**:
Run the `lora_tx.cpp` example as stated in step one using:
`sudo ./lora_tx.cpp`
This should show a bunch of GPS data or coordinates.

3b. **Client and Server Handshaking**:
Then run the `gps_rx_client.cpp` code using:
`sudo ./gps_rx_client.cpp`
This should show a bunch of success mesages for initialization, socket creation, server connection, GPS data sent, and a message from the server.

After this the person running the server should run `server.cpp` using:
`sudo ./server.cpp`
The server side terminal should show a bunch of success messages for socket creation, binding, listening for connection, connection accepted,
and the message from the client which should be one line of GPS coordinates sent over Wi-Fi through socket programing!

It should also show that the hello message from the server was sent to the client. 
