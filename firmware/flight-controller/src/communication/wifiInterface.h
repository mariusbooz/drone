#pragma once

#include "communicationInterface.h"
#include "../config/config.h"

#include <WiFi.h>
#include <WiFiUdp.h>


class WiFiInterface : public CommunicationInterface{
    public: 
        bool isConnected() override;

        bool begin(const char* ssid, const char* password);
        bool send(const uint8_t* message, size_t size) override;
        size_t receive(uint8_t* message, size_t size) override;



    private:
        WiFiUDP Udp;
        IPAddress pcIP;
        unsigned int pcPort = 0;
        bool connected = false;
};