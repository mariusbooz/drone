#include "MPU6050.h"


void MPU6050::setInterface(I2CInterface* newpI2CInterface){
    //enable I2C interface and establish connection
    pI2CInterface = newpI2CInterface;
    pI2CInterface->begin(sdaPin, sdlPin, mpuAddress);
}

void MPU6050::powerOn(){
    //set power management register 0x6B to 0x00 -> power MPU on.
    const uint8_t txMessage[2] = {0x6B, 0x00};
    size_t txSize = 2;
    pI2CInterface->send(txMessage, txSize);
}

void MPU6050::setGyroRange(uint8_t range){
    //set gyro range via register 0x1B
    const uint8_t txMessage[2] = {0x1B, static_cast<uint8_t>(angularVelocityRange)};
    size_t txSize = 2;
    pI2CInterface->send(txMessage, txSize);
}

void MPU6050::setAccelerationRange(uint8_t range){
    //set gyro range via register 0x1B
    const uint8_t txMessage[2] = {0x1C, static_cast<uint8_t>(accelerationRange)};
    size_t txSize = 2;
    pI2CInterface->send(txMessage, txSize);
}

void MPU6050::activate(){
    powerOn();
    setGyroRange(static_cast<uint8_t>(angularVelocityRange));
    setAccelerationRange(static_cast<uint8_t>(accelerationRange));
}

void MPU6050::reset(){
    //set power management register 1 to 0x80 -> reset MPU.
    const uint8_t txMessage[2] = {0x6B, 0x80};
    size_t txSize = 2;
    pI2CInterface->send(txMessage, txSize);
}

float MPU6050::convertAcceleration(int16_t acceleration){
    //values taken from MPU6050 register description, output in [m/s2]
    switch (accelerationRange){
        case SensorRange::LOW_SENSOR_RANGE:
            return ((float)acceleration / 16384.00) * LITTLE_G;

        case SensorRange::MEDIUM_SENSOR_RANGE:
            return ((float)acceleration / 8192.00) * LITTLE_G;

        case SensorRange::HIGH_SENSOR_RANGE:
            return ((float)acceleration / 4096.00) * LITTLE_G;
        
        case SensorRange::VERY_HIGH_SENSOR_RANGE:
            return ((float)acceleration / 2048.00) * LITTLE_G;
    }
}

float MPU6050::convertTemperature(int16_t temperature){
    //formula taken from MPU6050 register description, output in [C]
    return ((float)temperature / 340.00) + 36.53;
}

float MPU6050::convertAngularVelocity(int16_t angularVelocity){
    //values taken from MPU6050 register description, output in [deg/s]
    switch (angularVelocityRange){
        case SensorRange::LOW_SENSOR_RANGE:
            return ((float)angularVelocity / 131.00);

        case SensorRange::MEDIUM_SENSOR_RANGE:
            return ((float)angularVelocity / 65.50);

        case SensorRange::HIGH_SENSOR_RANGE:
            return ((float)angularVelocity / 32.80);
        
        case SensorRange::VERY_HIGH_SENSOR_RANGE:
            return ((float)angularVelocity / 16.40);
    }
}

IMUData MPU6050::readSensor(){
    //struct to return
    IMUData newData;

    //buffers to send and receive into, start reading 14 bytes starting from register 0x3B
    const uint8_t txMessage[1] = {0x3B};
    size_t txSize = 1;
    uint8_t rxMessage[14]; // axh,axl,ayh,ayl,azh,azl,
                           // th,tl
                           // gxh,gxl,gyh,gyl,gzh,gzl,
    size_t rxSize = 14;

    //read data from buffers bytewise into rxMessage
    size_t receivedBytes = pI2CInterface->sendReceive(txMessage, txSize, rxMessage, rxSize);

    //combine high and low bits 
    int16_t sensorValues[7]; //ax,ay,az,t,gx,gy,gz
    for (int i = 0; i < 7; i++) {
        sensorValues[i] = ((int16_t)rxMessage[2 * i] << 8) |
                (int16_t)rxMessage[2 * i + 1];
    }

    //populate datastruct and return
    newData.timestamp_us = micros();

    newData.accel_x = convertAcceleration(sensorValues[0]);
    newData.accel_y = convertAcceleration(sensorValues[1]);
    newData.accel_z = convertAcceleration(sensorValues[2]);

    newData.temp = convertTemperature(sensorValues[3]);

    newData.gyro_x = convertAngularVelocity(sensorValues[4]);
    newData.gyro_y = convertAngularVelocity(sensorValues[5]);
    newData.gyro_z = convertAngularVelocity(sensorValues[6]);

    return newData;
}