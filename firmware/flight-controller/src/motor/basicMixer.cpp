#include "motorMixer.h"

class BasicMixer : public MotorMixer {
    public:
        void mix(const ControlOutput& output) override;
        void getMotorOutput(MotorThrust& thrust) override;
    
    private:
        void ThrustSaturation() override;
        float thrustSetpoints[4]; // roll, pitch, yaw, thrust
        float thrustOutputs[4]; // FL, FR, BL, BR 
};

//Mixing function: takes control setpoints and applys mixing matrix
void BasicMixer::mix(const ControlOutput& output) {
    thrustSetpoints[0] = output.rollTorque;
    thrustSetpoints[1] = output.pitchTorque;
    thrustSetpoints[2] = output.yawTorque;
    thrustSetpoints[3] = output.thrust;

    for (int i = 0; i < 4; ++i) {
        thrustOutputs[i] = 0.0f;
        for (int j = 0; j < 4; ++j) {
            thrustOutputs[i] += MIXING_MATRIX[i][j] * thrustSetpoints[j];
        }
    }
    ThrustSaturation();
}

//Simple Clipping to 0 or MAX_MOTOR_OUTPUT, can be improved with more advanced methods like scaling or redistribution
void BasicMixer::ThrustSaturation() {
    for (int i = 0; i < 4; ++i) {
        if (thrustOutputs[i] > MAX_MOTOR_OUTPUT) {
            thrustOutputs[i] = MAX_MOTOR_OUTPUT;
        } else if (thrustOutputs[i] < 0.0f) {
            thrustOutputs[i] = 0.0f;
        }
    }
}

//get the final motor thrust values
void BasicMixer::getMotorOutput(MotorThrust& thrust) {
    thrust.frontLeft = thrustOutputs[0];
    thrust.frontRight = thrustOutputs[1];
    thrust.backLeft = thrustOutputs[2];
    thrust.backRight = thrustOutputs[3];
}