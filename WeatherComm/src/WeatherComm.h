// WeatherComm.h - Weather Station Communication Library
#ifndef WEATHER_COMM_H
#define WEATHER_COMM_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

/**
 * @brief Weather Station Communication Library using ESP-NOW
 * 
 * This library facilitates communication between weather station nodes (ESP32 devices)
 * and a central receiver (LilyGO) that collects data and forwards it via cellular connection.
 */
class WeatherComm {
  public:
    /**
     * @brief Initialize the communication system
     * @param teamName Identifier for your weather station team/node
     * @param receiverMac MAC address of the LilyGO receiver. If NULL, uses the value from CONFIG_RECEIVER_MAC
     * @return true if initialization was successful
     */
    bool begin(const char* teamName, uint8_t* receiverMac = NULL);
    
    /**
     * @brief Send weather data to the central receiver
     * @param message Weather data or command to send
     * @return true if message was sent successfully
     */
    bool sendData(const char* message);
    
    /**
     * @brief Set or update the receiver MAC address
     * @param macAddress 6-byte MAC address of the receiver
     */
    void setReceiverMAC(uint8_t* macAddress);
    
  private:
    char _teamName[32];
    uint8_t _receiverMac[6];
    
    // ESP-NOW message structure
    typedef struct struct_message {
        char team[32];      // Team name
        char message1[64];  // Message content
    } struct_message;
    
    struct_message _outgoingData;
    esp_now_peer_info_t _peerInfo;
    
    // Static callback handler
    static void _onDataSentCallback(const uint8_t *mac_addr, esp_now_send_status_t status);
};

#endif
