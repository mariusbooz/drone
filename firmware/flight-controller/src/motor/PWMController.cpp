#include "motorController.h"
#include "../config/config.h"

#include "Arduino.h"


class PWMController : public MotorController{
    public:
        void setThrust(MotorThrust thrust) override;
        void stopMotors() override;
        void startMotors() override;

    private:
        bool motorsActive = false;
        int speedM1 = 0, speedM2 = 0, speedM3 = 0, speedM4 = 0;
        int baseThrust = 0;
};

void PWMController::setThrust(MotorThrust thrust){
    //start motors if not done so already
    if (!motorsActive){
        startMotors();
    }
    
    //get thrust values and convert to correct int value, clamp for safety reasons
    speedM1 = constrain((int)(thrust.frontLeft * 255.00), 0, 255);
    speedM2 = constrain((int)(thrust.frontRight * 255.00), 0, 255);
    speedM3 = constrain((int)(thrust.backLeft * 255.00), 0, 255);
    speedM4 = constrain((int)(thrust.backRight * 255.00), 0, 255);

    //send PWM signal to the pins
    analogWrite(M1_IN1, speedM1);
    analogWrite(M2_IN1, speedM2);
    analogWrite(M3_IN1, speedM3);
    analogWrite(M4_IN1, speedM4);   
}

void PWMController::stopMotors(){
    //set all motor speed to zero
    speedM1 = speedM2 = speedM3 = speedM4 = 0;
    analogWrite(M1_IN1, 0); analogWrite(M2_IN1, 0);
    analogWrite(M3_IN1, 0); analogWrite(M4_IN1, 0);

    //set flag to false
    motorsActive = false;
}

void PWMController::startMotors(){
    //define correct motor pins as given in config
    pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
    pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
    pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
    pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);

    //set correct motor direction and set thrust to zero
    digitalWrite(M1_IN2, LOW); digitalWrite(M2_IN2, LOW);
    digitalWrite(M3_IN2, LOW); digitalWrite(M4_IN2, LOW);
    stopMotors();
}