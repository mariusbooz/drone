#pragma once

#include "../config/config.h"
#include "shared/datastructs.h"

class MotorMixer {
    public:
        virtual void mix(const ControlOutput& output) = 0;
        virtual void getMotorOutput(MotorThrust& thrust) = 0;
    
    private:
        virtual void ThrustSaturation() = 0;
};