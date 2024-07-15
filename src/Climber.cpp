#include <Arduino.h>
#include "Climber.h"

Climber::Climber(NoU_Servo* LeftClimber, NoU_Servo* RightClimber, State* state) : leftServo(LeftClimber), rightServo(RightClimber), robotState(state)
{ }

uint8_t Climber::begin(){
    leftServo->write(leftStowAngle);
    rightServo->write(rightStowAngle);

    return 0;
}

uint8_t Climber::update(){
    if(leftServo->getDegrees() != leftSetAngle){
        leftServo->write(leftSetAngle);
    }
    if(rightServo->getDegrees() != rightSetAngle){
        rightServo->write(rightSetAngle);
    }

    if(!robotState->isEnabled()){
        leftServo->write(leftStowAngle);
        rightServo->write(rightStowAngle);
    }

    return 0;
}

uint8_t Climber::getMode(){
    return climberMode;
}

void Climber::stow(){
    climberMode = 0;
    leftSetAngle = leftStowAngle;
    rightSetAngle = rightStowAngle;
}

void Climber::deploy(){
    climberMode = 1;
    leftSetAngle = leftDeployAngle;
    rightSetAngle = rightDeployAngle;
}

void Climber::set(double leftAngle, double rightAngle){
    climberMode = 2;
    leftSetAngle = leftAngle;
    rightSetAngle = rightAngle;
}

