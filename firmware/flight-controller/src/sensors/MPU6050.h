#pragma once

#include "communication/i2cInterface.h"
#include "shared/datastructs.h"
#include "../config/config.h"

//IMU data
constexpr uint8_t MPU6050_ADDRESS = 0x68; //pin AD0 pulled low, else 0x69
enum class SensorRange : uint8_t{
    LOW_SENSOR_RANGE = 0b00000000,
    MEDIUM_SENSOR_RANGE = 0b00001000,
    HIGH_SENSOR_RANGE = 0b00010000,
    VERY_HIGH_SENSOR_RANGE = 0b00011000
};


class MPU6050{
    public:
        void setInterface(I2CInterface* pI2CInterface);
        void powerOn();
        void setGyroRange(uint8_t range);
        void setAccelerationRange(uint8_t range);
        void activate();

        void reset();

        float convertAcceleration(int16_t acceleration);
        float convertTemperature(int16_t temperature);
        float convertAngularVelocity(int16_t angularVelocity);

        IMUData readSensor();
    private:
        uint8_t mpuAddress = MPU6050_ADDRESS;
        SensorRange angularVelocityRange = SensorRange::HIGH_SENSOR_RANGE;
        SensorRange accelerationRange = SensorRange::HIGH_SENSOR_RANGE;

        uint8_t sdaPin = I2C_SDA;
        uint8_t sdlPin = I2C_SDL;

        I2CInterface* pI2CInterface;
};