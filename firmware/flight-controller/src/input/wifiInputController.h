#pragma once

#include "inputController.h"
#include "../communication/wifiInterface.h"


class WiFiInputController : public InputController{
    public:
        void update() override;
        InputCommand getInput() override;

        void setInterface(WiFiInterface* pWiFiInterface);
        InputCommand InputCommandFromByteBuffer(uint8_t* message);

    private:
        uint8_t inputBuffer[32];
        size_t inputSize = 32;

        InputCommand latestInput;
        uint32_t sequence = 0;
        uint32_t missedPackages = 0;

        WiFiInterface* pWiFiInterface;
};