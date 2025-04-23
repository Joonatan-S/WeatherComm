/**
 * WeatherComm Library - Basic Weather Station Example
 * 
 * This example shows how to set up a basic weather station node
 * that sends data to a central LilyGO receiver.
 * 
 * Before using:
 * 1. Update the RECEIVER_MAC with your LilyGO MAC address
 * 2. Set your team/node name
 */

#include <WeatherComm.h>

// Set your LilyGO receiver's MAC address here
uint8_t RECEIVER_MAC[] = {0x08, 0xB6, 0x1F, 0xBE, 0x69, 0xF4}; 

// Alternatively, you can define it at compile time:
// arduino-cli compile -D "CONFIG_RECEIVER_MAC={0x08, 0xB6, 0x1F, 0xBE, 0x69, 0xF4}"

// Create WeatherComm instance
WeatherComm weatherStation;

// Your team/node name
const char* TEAM_NAME = "Node01";

// Example sensor values
float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;

// Message buffer
char message[64];

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize weather station communication
  // Pass your LilyGO receiver's MAC address
  if (!weatherStation.begin(TEAM_NAME, RECEIVER_MAC)) {
    Serial.println("Failed to initialize WeatherComm!");
    while (1) { delay(1000); } // Halt if initialization fails
  }
  
  // If you want to change the MAC address later:
  // uint8_t newMAC[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  // weatherStation.setReceiverMAC(newMAC);
}

void loop() {
  // Simulate reading from sensors
  temperature = random(1500, 3000) / 100.0; // 15.00 to 30.00 °C
  humidity = random(3000, 9000) / 100.0;    // 30.00 to 90.00 %
  pressure = random(9800, 10300) / 10.0;    // 980.0 to 1030.0 hPa
  
  // Format data into a message
  snprintf(message, sizeof(message), "T:%.2f,H:%.2f,P:%.1f", 
           temperature, humidity, pressure);
  
  // Send data
  if (weatherStation.sendData(message)) {
    Serial.println("Weather data sent successfully!");
  } else {
    Serial.println("Failed to send weather data!");
  }
  
  // Wait before sending next data packet
  delay(30000); // Send every 30 seconds
}
