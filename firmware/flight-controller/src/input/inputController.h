#pragma once

#include "shared/datastructs.h"

class InputController{
    public:
        virtual void update() = 0;
        virtual InputCommand getInput() = 0;
};