#include <Arduino.h>
#include "Climber.h"

#include "Constants.h"

Climber::Climber(NoU_Servo* ClimberServo, NoU_Servo* ReactionClimber, State* state) : climberServo(ClimberServo), reactionServo(ReactionClimber), robotState(state)
{ }

uint8_t Climber::begin(){
    climberServo->write(climberStowAngle);
    reactionServo->write(reactionStowAngle);

    return 0;
}

uint8_t Climber::update(){
    if(climberServo->getDegrees() != climberSetAngle){
        climberServo->write(climberSetAngle);
    }
    if(reactionServo->getDegrees() != reactionSetAngle){
        reactionServo->write(reactionSetAngle);
    }

    return 0;
}

uint8_t Climber::getMode(){
    return climberMode;
}

void Climber::stowClimber(){
    climberMode = 0;
    climberServo->write(climberStowAngle);
}

void Climber::deployClimber(){
    climberMode = 1;
    climberServo->write(climberDeployAngle);
}

void Climber::stowReaction(){
    climberMode = 0;
    reactionServo->write(reactionStowAngle);
}

void Climber::deployReaction(){
    climberMode = 1;
    reactionServo->write(reactionDeployAngle);
}

void Climber::set(double climberAngle, double reactionAngle){
    climberMode = 2;
    climberServo->write(climberAngle);
    reactionServo->write(reactionAngle);
}

