// WeatherComm.cpp - Implementation for Weather Station Communication Library
#include "WeatherComm.h"

// -----------------------------------------------------------------------------
// CONFIGURATION
// -----------------------------------------------------------------------------
// Default receiver MAC - Replace this with your LilyGO MAC address before using
// You can find your LilyGO MAC address by running the FindReceiverMAC example
// Format: {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} where XX are hexadecimal values
// -----------------------------------------------------------------------------
#ifndef CONFIG_RECEIVER_MAC
uint8_t defaultReceiverMac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#else
uint8_t defaultReceiverMac[] = CONFIG_RECEIVER_MAC;
#endif

// Static callback handler for ESP-NOW
void WeatherComm::_onDataSentCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\nSending to MAC: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivered ✓" : "Failed ✗");
}

bool WeatherComm::begin(const char* teamName, uint8_t* receiverMac) {
    // Save team name
    strncpy(_teamName, teamName, sizeof(_teamName) - 1);
    _teamName[sizeof(_teamName) - 1] = '\0'; // Ensure null termination
    
    // If no MAC provided, use default
    if (receiverMac == NULL) {
        // Check if default MAC is still the placeholder
        bool isDefaultMAC = true;
        for (int i = 0; i < 6; i++) {
            if (defaultReceiverMac[i] != 0x00) {
                isDefaultMAC = false;
                break;
            }
        }
        
        if (isDefaultMAC) {
            Serial.println("\n⚠️ WARNING: Using placeholder MAC address. Please configure your LilyGO receiver MAC address! ⚠️");
            Serial.println("Set a valid MAC address using the setReceiverMAC() method or update the defaultReceiverMac array.");
        }
        
        memcpy(_receiverMac, defaultReceiverMac, 6);
    } else {
        memcpy(_receiverMac, receiverMac, 6);
    }
    
    // Make sure Serial is initialized
    if (!Serial) {
        Serial.begin(115200);
        delay(100);
    }
    
    Serial.println("\n--- Initializing Weather Station Communication ---");
    Serial.printf("Team: %s\n", _teamName);
    
    // Display receiver MAC address
    Serial.print("Receiver MAC Address: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", _receiverMac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    
    // Ensure WiFi is in correct mode
    WiFi.disconnect();
    delay(200);
    WiFi.mode(WIFI_STA);
    delay(500);
    
    Serial.print("Sender MAC Address: ");
    Serial.println(WiFi.macAddress());
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return false;
    }
    
    // Register callback
    esp_now_register_send_cb(_onDataSentCallback);
    
    // Configure peer
    memcpy(_peerInfo.peer_addr, _receiverMac, 6);
    _peerInfo.channel = 0;
    _peerInfo.encrypt = false;
    
    // Add peer
    if (esp_now_add_peer(&_peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return false;
    }
    
    Serial.println("ESP-NOW communication initialized successfully");
    return true;
}

void WeatherComm::setReceiverMAC(uint8_t* macAddress) {
    // Update stored MAC
    memcpy(_receiverMac, macAddress, 6);
    
    // Update peer info
    esp_now_del_peer(_peerInfo.peer_addr); // Remove old peer
    memcpy(_peerInfo.peer_addr, macAddress, 6);
    esp_now_add_peer(&_peerInfo); // Add updated peer
    
    Serial.print("Receiver MAC updated to: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", _receiverMac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
}

bool WeatherComm::sendData(const char* message) {
    // Prepare data packet
    strncpy(_outgoingData.team, _teamName, sizeof(_outgoingData.team) - 1);
    _outgoingData.team[sizeof(_outgoingData.team) - 1] = '\0'; // Ensure null termination
    
    strncpy(_outgoingData.message1, message, sizeof(_outgoingData.message1) - 1);
    _outgoingData.message1[sizeof(_outgoingData.message1) - 1] = '\0'; // Ensure null termination
    
    // Debug output
    Serial.println("\nSending message...");
    Serial.printf("Team: %s\n", _outgoingData.team);
    Serial.printf("Message: %s\n", _outgoingData.message1);
    
    // Send using ESP-NOW
    esp_err_t result = esp_now_send(_receiverMac, (uint8_t*)&_outgoingData, sizeof(_outgoingData));
    
    if (result == ESP_OK) {
        Serial.println("Message sent successfully");
        return true;
    } else {
        Serial.println("Error sending message");
        return false;
    }
}
