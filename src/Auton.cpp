#include <Arduino.h>
#include "Auton.h"

uint8_t Auton::begin(){


    return 0;
}

uint8_t Auton::update(){
    
    if(autonEnabled){
        // taxi
        if(autonSelected == 1){
            if(firstRun){
                drivetrain->LinearHeadingDrive(taxiDist);
                firstRun = false;
            }
            if(drivetrain->autoFinished()){
                routineFinished = true;
                return 1;
            }
        }
        // preload
        else if(autonSelected == 2){
            if(firstRun){
                shooter->subShot();
                timer(500);
                firstRun = false;
            } 
            // this second thing cannot happen on the same 
            // update() call as the first thing
            else if(!timerStarted && !intake->autoFinished()){ // signifies timer being done
                intake->runUntilEmpty();
            }
            if(intake->autoFinished()){
                timer(500);
            }
            if(!timerStarted && intake->autoFinished()){
                routineFinished = 1;
                return 1;
            }
        }
        // preload + taxi
        else if(autonSelected == 3){
            if(routineCount == 0){
                preload();
                routineCount++;
            }
            else if(routineCount == 1 && routineFinished){
                taxi(taxiDist);
                routineCount++;
            }
            else if(routineCount == 2 && routineFinished){
                routineFinished = true;
                return 1;
            }
        }

    }
    
    return 0;
}

uint8_t Auton::timer(int milliseconds){
    if(!timerStarted){
        timerStarted = true;
        timeStart = millis();
    }
    if(millis() - timeStart > milliseconds){
        timerStarted = false;
        return 1;
    }
    return 0;
}

void Auton::taxi(double mm){
    autonSelected = 1;
    routineFinished = false;
    routineCount = 0;
    taxiDist = mm;
}

void Auton::preload(){
    autonSelected = 2;
    routineFinished = false;
    routineCount = 0;
}

void Auton::preloadTaxi(double mm){
    autonSelected = 3;
    routineFinished = false;
    routineCount = 0;
    taxiDist = mm;
}