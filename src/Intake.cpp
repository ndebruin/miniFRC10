#include <Arduino.h>
#include "Intake.h"

Intake::Intake(NoU_Motor* IntakeMotor, VL53L0X* tofSensor, State* state) : intakeMotor(IntakeMotor), sensor(tofSensor), robotState(state)
{ }

uint8_t Intake::begin(){
    intakeMotor->setInverted(true);

    // Wire.begin();

    // sensor->setTimeout(500);
    // if(!sensor->init()){
    //     return 1;
    // }
    
    // // reduce timing budget to 20ms per measurement for higher speed readings
    // sensor->setMeasurementTimingBudget(20000);

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
        if(hasNote()){
            stop();
            return 1;
        }
    }

    if(intakeMode == 3){
        if(!hasNote()){
            stop();
            return 1;
        }
    }

    return 0;
}

uint16_t Intake::getRange(){
    // return sensor->readRangeSingleMillimeters();
    return 50;
}

uint8_t Intake::getMode(){
    return intakeMode;
}

// this will likely have to be tuned / rewritten
bool Intake::hasNote(){
    if(getRange() < Distance_EMPTY){
        robotState->setNote(true);
        return true;
    }
    robotState->setNote(false);
    return false;
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
    intakeMode = 2;
    setPower = Intake_IN_kS;
}

void Intake::runUntilEmpty(){
    intakeMode = 3;
    setPower = Intake_SHOOT_kS;
}