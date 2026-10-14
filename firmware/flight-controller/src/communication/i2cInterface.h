#pragma once

#include "communicationInterface.h"

#include "Wire.h"


class I2CInterface : public CommunicationInterface{
    public:
        bool begin(uint8_t sda, uint8_t sdl, uint8_t newAddress);
        bool isConnected() override;
        void setAddress(uint8_t newAddress);

        bool send(const uint8_t* txMessage, size_t txSize) override;
        size_t receive(uint8_t* rxMessage, size_t rxSize) override;
        size_t sendReceive(const uint8_t* txMessage, size_t txSize, uint8_t* rxMessage, size_t rxSize);

    private:
        uint8_t address;
        bool connected = false;
};