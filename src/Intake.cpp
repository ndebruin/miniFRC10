#include <Arduino.h>
#include "Intake.h"

Intake::Intake(NoU_Motor* IntakeMotor, State* state) : intakeMotor(IntakeMotor), robotState(state)
{ }

uint8_t Intake::begin(){
    intakeMotor->setInverted(true);


    stop();

    return 0;
}

uint8_t Intake::update(){
    if(robotState->isEnabled()){
        if(intakeMotor->getOutput() != setPower){
            intakeMotor->set(setPower);
        }
    }

    if(intakeMode == 2){
        if(robotState->hasNote()){
            stop();
            autonFinished = true;
            return 1;
        }
    }

    if(intakeMode == 3){
        if(!robotState->hasNote()){
            stop();
            autonFinished = true;
            return 1;
        }
    }

    return 0;
}

uint8_t Intake::getMode(){
    return intakeMode;
}

void Intake::stop(){
    intakeMode = 0;
    setPower = 0.0;
}

void Intake::run(){
    intakeMode = 1;
    setPower = Intake_IN_kS;
}

void Intake::outtake(){
    intakeMode = 1;
    setPower = -Intake_IN_kS;
}

void Intake::run(double kS){
    intakeMode = 1;
    setPower = kS;
}

void Intake::runUntilBreak(){
    autonFinished = false;
    intakeMode = 2;
    setPower = Intake_IN_kS;
}

void Intake::runUntilEmpty(){
    autonFinished = false;
    intakeMode = 3;
    setPower = Intake_SHOOT_kS;
}