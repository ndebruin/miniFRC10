#ifndef Auton_h
#define Auton_h

#include <Arduino.h>

#include "Drivetrain.h"
#include "Intake.h"
#include "Shooter.h"
#include "State.h"

#include <Constants.h>

class Auton
{
    public:
        Auton(Drivetrain* Drivetrain, Intake* Intake, Shooter* Shooter, State* state) 
            : drivetrain(Drivetrain) , intake(Intake), shooter(Shooter), robotState(state){}


        uint8_t begin();
        uint8_t update();

        uint8_t getAuton(){return autonSelected;}

        void setAuton(uint8_t autonSelect){autonSelected = autonSelect;}

        void enableAuton(){autonEnabled = true;}
        void disableAuton(){autonEnabled = false;}

        // actual routines
        void taxi(double mm);
        void preload();
        void preloadTaxi(double mm);



    private:
        Drivetrain* drivetrain;
        Intake* intake;
        Shooter* shooter;
        State* robotState;

        bool autonEnabled;

        // specific routines
        uint8_t timer(int milliseconds);
        
        double taxiDist;
        
        
        // 0 - None
        // 1 - Preload
        // 2 - Taxi
        // 3 - preload + taxi
        uint8_t autonSelected;

        // timer specifics
        unsigned long timeStart;
        bool timerStarted;
        bool routineFinished = false;

        bool firstRun = true;

        uint8_t routineCount = 0;
        
};

#endif