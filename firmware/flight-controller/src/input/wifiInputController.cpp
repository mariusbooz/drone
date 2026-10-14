#include "wifiInputController.h"
#include "../utils/utils.h"

//flags
constexpr uint8_t ARM = 1 << 0;
constexpr uint8_t DISARM = 1 << 1;

void WiFiInputController::update(){
    //receive next package
    size_t bytesReceived = pWiFiInterface->receive(inputBuffer, inputSize);

    //check if we have received enough bytes, else return
    if (bytesReceived != 19){
        return;
    }

    //check first two bytes to identify correct start of package, return if magic invalid
    uint16_t magic = ((uint16_t)inputBuffer[0] << 8) | (uint16_t)inputBuffer[1];
    if(magic != 0xCAFE){
        return;
    }

    //check type to check for corrrect package type
    if (inputBuffer[3] != 1){
        return;
    }

    //compute crc and see if it fits, else corrupted data and return
    uint16_t crc = ((uint16_t)inputBuffer[17] << 8) | (uint16_t)inputBuffer[18];
    //calculate crc only over the rest of the field, not the sent crc value
    if(calculateCRC16(inputBuffer, bytesReceived - 2) != crc){
        return;
    }

    //we have a correct package, convert it to data struct and save if most recent data
    InputCommand receivedInputCommand = InputCommandFromByteBuffer(inputBuffer);

    if(receivedInputCommand.sequence > sequence){
        uint32_t packagesInbetween =  receivedInputCommand.sequence - sequence;
        missedPackages = missedPackages + packagesInbetween - 1;
        sequence = receivedInputCommand.sequence;
        latestInput = receivedInputCommand;
    }
    return;
}

InputCommand WiFiInputController::getInput(){
    return latestInput;
}

void WiFiInputController::setInterface(WiFiInterface* pNewWiFiInterface){
    pWiFiInterface = pNewWiFiInterface;
}

InputCommand WiFiInputController::InputCommandFromByteBuffer(uint8_t* message){
    InputCommand newInputCommand;

    //fill in new data from byte stream
    //timestamp
    newInputCommand.timestamp_us = micros();

    //sequence
    uint32_t firstByte = (uint32_t)inputBuffer[4] << 24;
    uint32_t secondByte = (uint32_t)inputBuffer[5] << 16;
    uint32_t thirdByte = (uint32_t)inputBuffer[6] << 8;
    uint32_t fourthByte = (uint32_t)inputBuffer[7];
    
    newInputCommand.sequence = firstByte | secondByte | thirdByte | fourthByte;

    //Movement commands
    newInputCommand.roll = (((int16_t)inputBuffer[8] << 8) | (int16_t)inputBuffer[9]) / 10000.00;
    newInputCommand.pitch = (((int16_t)inputBuffer[10] << 8) | (int16_t)inputBuffer[11]) / 10000.00;
    newInputCommand.yaw = (((int16_t)inputBuffer[12] << 8) | (int16_t)inputBuffer[13]) / 10000.00;
    newInputCommand.thrust = (((int16_t)inputBuffer[14] << 8) | (int16_t)inputBuffer[15]) / 10000.00;

    //flags
    newInputCommand.arm = inputBuffer[16] & ARM;
    newInputCommand.disarm = inputBuffer[16] & DISARM;

    return newInputCommand;
}