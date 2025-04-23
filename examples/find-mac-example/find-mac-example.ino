/**
 * WeatherComm Library - Find Receiver MAC Address Example
 * 
 * This sketch should be uploaded to your LilyGO device
 * to determine its MAC address for use with WeatherComm.
 */

#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);
  delay(500);
  
  Serial.println("\n=============================================");
  Serial.println("WeatherComm - LilyGO MAC Address Finder");
  Serial.println("=============================================");
  
  // Get and display the MAC address
  String macStr = WiFi.macAddress();
  Serial.println("MAC Address: " + macStr);
  
  // Convert string format to byte array format for easy copy-paste
  Serial.println("\nCopy this line to your code:");
  Serial.print("uint8_t RECEIVER_MAC[] = {");
  
  int index = 0;
  for (int i = 0; i < macStr.length(); i += 3) {
    // Extract the hex value from the string
    String byteStr = macStr.substring(i, i+2);
    Serial.print("0x");
    Serial.print(byteStr);
    
    if (index < 5) {
      Serial.print(", ");
    }
    index++;
  }
  
  Serial.println("};");
  Serial.println("\nOr define it at compile time:");
  Serial.print("-D \"CONFIG_RECEIVER_MAC={");
  
  index = 0;
  for (int i = 0; i < macStr.length(); i += 3) {
    String byteStr = macStr.substring(i, i+2);
    Serial.print("0x");
    Serial.print(byteStr);
    
    if (index < 5) {
      Serial.print(", ");
    }
    index++;
  }
  
  Serial.println("}\"");
  Serial.println("=============================================");
}

void loop() {
  // Nothing to do in loop
  delay(10000);
}
