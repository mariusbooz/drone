#pragma once

#include <cstdint>
#include <stddef.h>
#include "utils/utils.h"

////////////////////////////////////////////////
//Data structures used in the flight controller
////////////////////////////////////////////////
//Sensor data structs
struct IMUData
{
    uint64_t timestamp_us;

    float accel_x;   // m/s²
    float accel_y;
    float accel_z;

    float temp;

    float gyro_x;    // rad/s
    float gyro_y;
    float gyro_z;
};

struct GPSData
{
    uint64_t timestamp_us;
};

struct BarometerData
{
    uint64_t timestamp_us;
};

//Current State of the drone, used for control and state estimation
struct State
{
    uint64_t timestamp_us;

    Quaternion attitude;

    float roll_rate;
    float pitch_rate;
    float yaw_rate;
};

//Output of the control algorithm, used for motor mixing and actuation
struct ControlOutput {
  float rollTorque; //-1.0 ... 1.0
  float pitchTorque;
  float yawTorque;

  float thrust; //0.0 ... 1.0
};

//This is desired thrust output for each motor, after mixing and scaling; 
struct MotorThrust {
  float frontLeft; // 0.0 ... 1.0
  float frontRight;
  float backLeft;
  float backRight;
};

//struct used to transmit as serialized uint8_t stream
struct InputCommandPacket
{
    uint16_t magic;
    uint8_t  version;
    uint8_t  type;

    uint32_t sequence;

    int16_t  roll;
    int16_t  pitch;
    int16_t  yaw;
    uint16_t thrust;

    uint8_t  flags;

    uint16_t crc;
};

//Input data struct
struct InputCommand
{
    uint64_t timestamp_us;

    uint32_t sequence;
    
    float roll;      // -1.0 ... 1.0
    float pitch;     // -1.0 ... 1.0
    float yaw;       // -1.0 ... 1.0
    float thrust;  // 0.0 ... 1.0

    bool arm;
    bool disarm;
};