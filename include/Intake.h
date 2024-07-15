#ifndef Intake_h
#define Intake_h

#include <Arduino.h>
#include <Alfredo_NoU2.h>
#include <VL53L0X.h>
#include <Wire.h>

#include "State.h"

#include "Constants.h"

class Intake
{
    public:
        Intake(NoU_Motor* IntakeMotor, VL53L0X* tofSensor, State* state);
        uint8_t begin();
        uint8_t update();

        uint8_t getMode();

        void runUntilBreak();
        void runUntilEmpty();
        void run(double kS);
        void run();
        void outtake();
        void stop();

        uint16_t getRange();

        bool hasNote();
    private:
        NoU_Motor* intakeMotor;
        VL53L0X* sensor;

        State* robotState;

        double setPower;

        // 0 - stop
        // 1 - open loop
        // 2 - intake
        // 3 - shoot
        uint8_t intakeMode;

};

#endif