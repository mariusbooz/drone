#pragma once

#include "shared/datastructs.h"

class MotorController{
    public:
        virtual void setThrust(MotorThrust thrust) = 0;
        virtual void stopMotors() = 0;
        virtual void startMotors() = 0;
};