#include <Arduino.h>
#include "Shooter.h"

Shooter::Shooter(NoU_Motor* TopMotor, NoU_Motor* BottomMotor, State* state) : topMotor(TopMotor), bottomMotor(BottomMotor), robotState(state)
{ }

uint8_t Shooter::begin(){
    topMotor->setInverted(false);
    bottomMotor->setInverted(true);

    return 0;
}

uint8_t Shooter::update(){
    if(robotState->isEnabled()){
        if(topMotor->getOutput() != topSetPower){
            topMotor->set(topSetPower);
        }
        if(bottomMotor->getOutput() != bottomSetPower){
            bottomMotor->set(bottomSetPower);
        }
    }

    // if we are shooting
    // if(shooterMode == 2 || shooterMode == 3 || shooterMode == 4 || shooterMode == 5){
    //     if(!robotState->hasNote() && !timerStarted){
    //         // start timer
    //         time = millis();
    //         timerStarted = true;
    //     }
    //     if(timerStarted && millis()-time > shooterDelay*1000){
    //         stop();
    //         timerStarted = false;
    //         return 1;
    //     }
    // }

    return 0;
}

uint8_t Shooter::getMode(){
    return shooterMode;
}

void Shooter::stop(){
    shooterMode = 0;
    topSetPower = 0.0;
    bottomSetPower = 0.0;
}

void Shooter::run(double kS_Top, double kS_Bottom){
    shooterMode = 1;
    topSetPower = kS_Top;
    bottomSetPower = kS_Bottom;
}

void Shooter::ampShot(){
    shooterMode = 2;
    topSetPower = AMP_Top_kS;
    bottomSetPower = AMP_Bottom_kS;
}

void Shooter::subShot(){
    shooterMode = 3;
    topSetPower = SUBWOOFER_Top_kS;
    bottomSetPower = SUBWOOFER_Bottom_kS;
}

void Shooter::podiumShot(){
    shooterMode = 4;
    topSetPower = PODIUM_Top_kS;
    bottomSetPower = PODIUM_Bottom_kS;
}

void Shooter::passingShot(){
    shooterMode = 5;
    topSetPower = PASS_Top_kS;
    bottomSetPower = PASS_Bottom_kS;
}

void Shooter::intake(){
    shooterMode = 6;
    topSetPower = INTAKE_Top_kS;
    bottomSetPower = INTAKE_Bottom_kS;
}