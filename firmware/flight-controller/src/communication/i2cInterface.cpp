#include "i2cInterface.h"


bool I2CInterface::begin(uint8_t sda, uint8_t sdl, uint8_t newAddress){
    address = newAddress;
    Wire.begin(sda, sdl);

    connected = true;
    return true;
}

bool I2CInterface::isConnected(){
    return connected;
}

void I2CInterface::setAddress(uint8_t newAddress){
    address = newAddress;
}

bool I2CInterface::send(const uint8_t* txMessage, size_t txSize){
    //begin communication on the wire
    Wire.beginTransmission(address);

    //write bytes to bus and close bus after
    size_t bytesSent = Wire.write(txMessage, txSize);
    uint8_t successfull = Wire.endTransmission(true);

    //return true, if entire message was sent
    return (bytesSent == txSize && successfull == 0);
}

size_t I2CInterface::receive(uint8_t* rxMessage, size_t rxSize){
    //request rxSize bytes, bytesAvailable <= rxSize bytes ready to be read 
    uint8_t bytesAvailable = Wire.requestFrom(address, rxSize, true);

    //read bytes one by one into message buffer
    size_t bufferIdx = 0;
    while(Wire.available()){
        rxMessage[bufferIdx] = Wire.read();
        bufferIdx++;
    }

    //return read bytes
    return bufferIdx;
}

size_t I2CInterface::sendReceive(const uint8_t* txMessage, size_t txSize, uint8_t* rxMessage, size_t rxSize){
    //begin communication on the wire
    Wire.beginTransmission(address);

    //write bytes to bus and leave bus open to receive
    size_t bytesSent = Wire.write(txMessage, txSize);
    uint8_t successfull = Wire.endTransmission(false);

    //now request rxSize bytes to be read
    uint8_t bytesAvailable = Wire.requestFrom(address, rxSize, true);

    //read bytes one by one into message buffer
    size_t bufferIdx = 0;
    while(Wire.available()){
        rxMessage[bufferIdx] = Wire.read();
        bufferIdx++;
    }

    //return read bytes
    return bufferIdx;
}