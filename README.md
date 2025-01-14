## ESP32 AES Encryption and Communication Project

# Overview
This project demonstrates a secure communication system using two ESP32 Dev Kit boards. The setup involves:

1. SENDER ESP32: 
   - Reads temperature and humidity values from a DHT11 sensor.
   - Encrypts the sensor data using the AES algorithm.
   - Sends the encrypted data to the RECEIVER ESP32.

2. RECEIVER ESP32:
   - Receives the encrypted messages from the SENDER ESP32.
   - Displays the encrypted messages.
   - Decrypts the received messages.
   - Displays the decrypted messages.

# Tools and Libraries
- Development Environment: Arduino IDE
- Cryptography Library: mbedtls

# Features
- Secure data encryption and decryption using AES.
- Real-time temperature and humidity monitoring with DHT11.
- Wireless communication between two ESP32 boards.


 
