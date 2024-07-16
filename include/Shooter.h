#ifndef Shooter_h
#define Shooter_h

#include <Arduino.h>
#include <Alfredo_NoU2.h>

#include "State.h"

#include "Constants.h"

class Shooter
{
    public:
        Shooter(NoU_Motor* TopMotor, NoU_Motor* BottomMotor, State* state);
        uint8_t begin();
        uint8_t update();


        // 0 - stop
        // 1 - custom open loop
        // 2 - amp
        // 3 - subwoofer
        // 4 - podium
        // 5 - passing
        // 6 - intake
        uint8_t getMode();

        void stop();
        void run(double kS_Top, double kS_Bottom);
        void ampShot();
        void subShot();
        void podiumShot();
        void passingShot();
        void intake();


    private:
        NoU_Motor* topMotor;
        NoU_Motor* bottomMotor;

        State* robotState;

        double topSetPower;
        double bottomSetPower;

        unsigned long time;
        bool timerStarted;

        // 0 - stop
        // 1 - custom open loop
        // 2 - amp
        // 3 - subwoofer
        // 4 - podium
        // 5 - passing
        // 6 - intake
        uint8_t shooterMode;

};

#endif