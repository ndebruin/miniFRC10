#ifndef Climber_h
#define Climber_h

#include <Arduino.h>
#include <Alfredo_NoU2.h>

#include "State.h"

#include "Constants.h"

class Climber
{
    public:
        Climber(NoU_Servo* LeftClimber, NoU_Servo* RightClimber, State* state);
        uint8_t begin();
        uint8_t update();


        // 0 - Stow
        // 1 - Deploy
        uint8_t getMode();

        void stow();
        void deploy();

        void set(double leftAngle, double rightAngle);


    private:
        NoU_Servo* leftServo;
        NoU_Servo* rightServo;

        State* robotState;

        double leftSetAngle;
        double rightSetAngle;

        // 0 - Stow
        // 1 - Deploy
        // 2 - Custom Angle
        uint8_t climberMode;

};

#endif