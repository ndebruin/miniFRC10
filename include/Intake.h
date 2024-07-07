#ifndef Intake_h
#define Intake_h

#include <Arduino.h>
#include <Alfredo_NoU2.h>
#include <VL53L0X.h>
#include <Wire.h>

#include "Constants.h"

class Intake
{
    public:
        Intake(NoU_Motor* IntakeMotor);
        uint8_t begin();
        uint8_t update();

        uint8_t getMode();

        void runUntilBreak();
        void runUntilEmpty();
        void run(double kS);
        void run();
        void stop();

        uint16_t getRange();

        bool hasNote();
    private:
        NoU_Motor* intakeMotor;
        VL53L0X sensor;

        double setPower;

        // 0 - stop
        // 1 - open loop
        // 2 - intake
        // 3 - shoot
        uint8_t intakeMode;

};

#endif