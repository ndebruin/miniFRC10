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
                firstRun = false;
                secondRun = true;
                timeStart = millis();
                
            }
            // this second thing cannot happen on the same 
            // update() call as the first thing
            else if(secondRun && millis() - timeStart > 1000){ // signifies timer being done
                intake->run();
                secondRun = false;
                timeStart = millis();
                routineFinished = true;
                return 1;
            }

        }
        // preload + taxi
        else if(autonSelected == 3){
            if(firstRun){
                shooter->subShot();
                firstRun = false;
                secondRun = true;
                timeStart = millis();
                
            }
            // this second thing cannot happen on the same 
            // update() call as the first thing
            else if(secondRun && millis() - timeStart > 2000){ // signifies timer being done
                intake->run();
                secondRun = false;
                timeStart = millis();
                
            }
            else if(!secondRun && millis() - timeStart > 2000){
                intake->stop();
                shooter->stop();
                drivetrain->LinearHeadingDrive(-25.4*taxiDistanceIn);
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
    firstRun = true;
    secondRun = true;
    routineCount = 0;
}

void Auton::preloadTaxi(double mm){
    autonSelected = 3;
    routineFinished = false;
    routineCount = 0;
    taxiDist = mm;
}