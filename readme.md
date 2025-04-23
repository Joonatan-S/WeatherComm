# WeatherComm Library

A lightweight Arduino library for ESP32-based weather stations to communicate with a central LilyGO receiver using ESP-NOW protocol.

## Overview

This library facilitates a network of ESP32 devices acting as weather stations that send data to a central LilyGO device, which then forwards the data via its cellular connection. The communication between nodes and the central receiver uses ESP-NOW for efficient, low-power communication.

## Key Features

- Simple setup for ESP-NOW communication between ESP32 devices
- Easy-to-use API for sending weather data
- Support for team/node identification
- Dynamic configuration of receiver MAC address

## Installation

1. Download the ZIP file or clone this repository
2. In Arduino IDE: Sketch > Include Library > Add .ZIP Library...
3. Select the downloaded ZIP file or the folder containing the library

## Getting Started

### Step 1: Find your LilyGO receiver's MAC address

Upload the `FindReceiverMAC` example to your LilyGO device to get its MAC address:

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(500);
  Serial.println("MAC Address: " + WiFi.macAddress());
}

void loop() {
  delay(10000);
}
```

### Step 2: Configure your weather station node

```cpp
#include <WeatherComm.h>

// Set your LilyGO receiver's MAC address here
uint8_t RECEIVER_MAC[] = {0x08, 0xB6, 0x1F, 0xBE, 0x69, 0xF4}; // Replace with actual MAC

// Create WeatherComm instance
WeatherComm weatherStation;

void setup() {
  Serial.begin(115200);
  
  // Initialize weather station communication
  weatherStation.begin("Node01", RECEIVER_MAC);
}

void loop() {
  // Read sensor data (example)
  float temperature = 25.5;
  float humidity = 65.3;
  
  // Format data
  char message[64];
  snprintf(message, sizeof(message), "T:%.1f,H:%.1f", temperature, humidity);
  
  // Send data
  weatherStation.sendData(message);
  
  delay(60000); // Send every minute
}
```

## Configuration Methods

### Method 1: Direct in code

Set the MAC address directly in your sketch:

```cpp
uint8_t RECEIVER_MAC[] = {0x08, 0xB6, 0x1F, 0xBE, 0x69, 0xF4};
weatherStation.begin("Node01", RECEIVER_MAC);
```

### Method 2: Compile-time definition

Define the MAC address at compile time:

```bash
arduino-cli compile -D "CONFIG_RECEIVER_MAC={0x08, 0xB6, 0x1F, 0xBE, 0x69, 0xF4}" YourSketch.ino
```

### Method 3: Runtime update

Update the MAC address during runtime:

```cpp
uint8_t newMAC[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
weatherStation.setReceiverMAC(newMAC);
```

## API Reference

### `bool begin(const char* teamName, uint8_t* receiverMac = NULL)`
Initialize the communication system with team name and optional receiver MAC address.

### `bool sendData(const char* message)`
Send weather data to the central receiver.

### `void setReceiverMAC(uint8_t* macAddress)`
Set or update the receiver MAC address.

## License

This library is released under the MIT License. See the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
