#pragma once

#include "shared/datastructs.h"

class CommunicationInterface{
    public:
        virtual bool send(const uint8_t* message, size_t size) = 0;
        virtual size_t receive(uint8_t* message, size_t size) = 0;

        virtual bool isConnected() = 0;
};