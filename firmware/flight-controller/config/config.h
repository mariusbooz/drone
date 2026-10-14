#pragma once

#include "WiFi.h"

//I2C data
constexpr uint8_t I2C_SDA = 21;
constexpr uint8_t I2C_SDL = 22;

//IMU data
constexpr float LITTLE_G = 9.80983; // Heidelberg [m/s2]

//Wlan data
constexpr char* WIFI_SSID     = "Autobots";
constexpr char* WIFI_PASSWORD = "XYyKdKyFe3vw8fFR";
constexpr unsigned int LOCAL_UDP_PORT = 4210;
constexpr unsigned int SERVER_PORT = 5000;
const IPAddress SERVER_IP = IPAddress(192, 168, 1, 100);

constexpr float MAX_WAITING_TIME = 5000;

//Rotation Directions
constexpr int MOTOR_FL_DIR = 1; //Front Left CW
constexpr int MOTOR_FR_DIR = -1; //Front Right CCW
constexpr int MOTOR_BL_DIR = -1; //Back Left CCW
constexpr int MOTOR_BR_DIR = 1; //Back Right CW

//Mixing matrix for 4 motors, (roll, pitch, yaw, thrust) -> (frontLeft, frontRight, backLeft, backRight)
constexpr float MIXING_MATRIX[4][4] = { 
    { -1.0f, -1.0f, -1.0f, 1.0f }, // FL
    { -1.0f,  1.0f,  1.0f, 1.0f }, // FR
    {  1.0f, -1.0f,  1.0f, 1.0f }, // BL
    {  1.0f,  1.0f, -1.0f, 1.0f } // BR
};

// Current motor pin layout
constexpr int M1_IN1 = 12; constexpr int M1_IN2 = 13; // Motor 1: Vorne Links
constexpr int M2_IN1 = 14; constexpr int M2_IN2 = 27; // Motor 2: Vorne Rechts
constexpr int M3_IN1 = 26; constexpr int M3_IN2 = 25; // Motor 3: Hinten Links
constexpr int M4_IN1 = 32; constexpr int M4_IN2 = 33; // Motor 4: Hinten Rechts

//Motor output limits
constexpr float MAX_MOTOR_OUTPUT = 1.0f; 
constexpr float MAX_ROLL_RATE = 180.0f; // deg/s
constexpr float MAX_PITCH_RATE = 180.0f; // deg/s
constexpr float MAX_YAW_RATE = 1.0f; // deg/s