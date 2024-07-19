#include <Arduino.h>
#include "Drivetrain.h"

Drivetrain::Drivetrain(NoU_Motor* LeftMotor, NoU_Motor* RightMotor, State* state) 
                                    : leftMotor(LeftMotor), rightMotor(RightMotor), robotState(state)
{ 
    //theta_Controller.SetSampleTime(50);
}

uint8_t Drivetrain::getMode(){
    return driveMode;
}

uint8_t Drivetrain::begin(){
    leftMotor->setInverted(false);
    rightMotor->setInverted(true);

    return 0;
    
}

// this should be run as fast as possible to allow for constant updates to yaw and control loops
uint8_t Drivetrain::update(){
    
    // if in closed loop control
    if(driveMode > 0){
        // update our yaw coordinate 
        current_Theta = robotState->getYaw();
        // find the mean of the two encoders (to account for turning actions)
        currentLeft = robotState->getLeftCount();
        currentRight = robotState->getRightCount();
    }

    // if we're in an open loop mode, we can't run the PID controllers.
    if(driveMode == 0){
        theta_Controller.SetMode(MANUAL);
        left_Controller.SetMode(MANUAL);
        right_Controller.SetMode(MANUAL);
    }

    // linear heading
    if(driveMode == 1){
        if(abs(ySetpoint-currentLeft) < y_ErrorThreshold && abs(ySetpoint-currentRight) < y_ErrorThreshold){
            cancelAuto();
            return 1;
        }
        left_Controller.Compute();
        right_Controller.Compute();

        int leftDirection = 1;
        if(currentLeft > ySetpoint){
            leftDirection = -1;
        }
        left_powerOut = leftDirection*(linY_kS + abs(left_Out));

        int rightDirection = 1;
        if(currentRight > ySetpoint){
            rightDirection = -1;
        }
        right_powerOut = rightDirection*(linY_kS + abs(right_Out));

        if(robotState->isEnabled()){
            leftMotor->set(left_powerOut);
            rightMotor->set(right_powerOut);
        }
    }

    // linear heading until intake
    if(driveMode == 4){
        if(robotState->hasNote() || (abs(ySetpoint-currentLeft) < y_ErrorThreshold && abs(ySetpoint-currentRight) < y_ErrorThreshold)){
            cancelAuto();
            return 1;
        }
        left_Controller.Compute();
        right_Controller.Compute();

        int leftDirection = 1;
        if(currentLeft > ySetpoint){
            leftDirection = -1;
        }
        left_powerOut = leftDirection*(linY_kS + abs(left_Out));

        int rightDirection = 1;
        if(currentRight > ySetpoint){
            rightDirection = -1;
        }
        right_powerOut = rightDirection*(linY_kS + abs(right_Out));

        if(robotState->isEnabled()){
            leftMotor->set(left_powerOut);
            rightMotor->set(right_powerOut);
        }
    }


    // turn to angle
    if(driveMode == 2){
        if(abs(theta_Setpoint-current_Theta) < theta_ErrorThreshold){
            cancelAuto();
            return 1;
        }

        theta_Controller.Compute();
        
        int direction = 1;
        if(current_Theta > theta_Setpoint){
            direction = -1;
        }

        angZ_powerOut = direction*(angZ_TURN_kS + abs(angZ_Out));

        ArcadeDrive(0, angZ_powerOut);
    }
    return 0;
}

// copied from Alfredo
void Drivetrain::ArcadeDrive(double linY, double angZ)
{

    float leftPower = 0;
    float rightPower = 0;
    float maxInput = (linY > 0 ? 1 : -1) * max(fabs(linY), fabs(angZ));
    if (linY > 0) {
        if (angZ > 0) {
            leftPower = maxInput;
            rightPower = linY - angZ;
        }
        else {
            leftPower = linY + angZ;
            rightPower = maxInput;
        }
    } else {
        if (angZ > 0) {
            leftPower = linY + angZ;
            rightPower = maxInput;
        }
        else {
            leftPower = maxInput;
            rightPower = linY - angZ;
        }
    }
    if(robotState->isEnabled()){
        leftMotor->set(leftPower);
        rightMotor->set(rightPower);
    }
}

// copied from Alfredo
void Drivetrain::ChezyDrive(double linY, double angZ, boolean isQuickTurn)
{

    float angularPower;
    boolean overPower;

    if (isQuickTurn) {
        if (fabs(linY) < quickStopThreshold) {
            quickStopAccumulator = (1 - quickStopAlpha) * quickStopAccumulator + quickStopAlpha * angZ * 2;
        }
        overPower = true;
        angularPower = angZ;
    }
    else {
        overPower = false;
        angularPower = fabs(linY) * angZ - quickStopAccumulator;

        if (quickStopAccumulator > 1) quickStopAccumulator--;
        else if (quickStopAccumulator < -1) quickStopAccumulator++;
        else quickStopAccumulator = 0;
    }

    float leftPower;
    float rightPower;

    leftPower = linY + angularPower;
    rightPower = linY - angularPower;

    if (overPower) {
        if (leftPower > 1) {
            rightPower -= leftPower - 1;
            leftPower = 1;
        } else if (rightPower > 1) {
            leftPower -= rightPower - 1;
            rightPower = 1;
        } else if (leftPower < -1) {
            rightPower -= leftPower + 1;
            leftPower = -1;
        } else if (rightPower < -1) {
            leftPower -= rightPower + 1;
            rightPower = -1;
        }
    }
    float maxMagnitude = max(fabs(leftPower), fabs(rightPower));
    if (maxMagnitude > 1) {
        leftPower /= maxMagnitude;
        rightPower /= maxMagnitude;
    }
    if(robotState->isEnabled()){
        leftMotor->set(leftPower);
        rightMotor->set(rightPower);
    }
}

// drive in the current direction with heading correction (kinda)
void Drivetrain::LinearHeadingDrive(double mm){
    driveMode = 1;

    resetEncoders();

    ySetpoint = mm*mmPerTick;

    // setup distance control loop
    left_Controller.SetTunings(linY_kP, linY_kI, linY_kD);
    right_Controller.SetTunings(linY_kP, linY_kI, linY_kD);
    left_Controller.SetOutputLimits(-linY_LIMIT,linY_LIMIT);
    right_Controller.SetOutputLimits(-linY_LIMIT,linY_LIMIT);
    left_Controller.Compute();
    right_Controller.Compute();
    left_Controller.SetMode(AUTOMATIC);
    right_Controller.SetMode(AUTOMATIC);
    
}

void Drivetrain::LinearHeadingDriveUntilIntake(double mm){
    driveMode = 4;

    resetEncoders();

    ySetpoint = mm*mmPerTick;

    // setup distance control loop
    left_Controller.SetTunings(linY_kP, linY_kI, linY_kD);
    right_Controller.SetTunings(linY_kP, linY_kI, linY_kD);
    left_Controller.SetOutputLimits(-linY_LIMIT,linY_LIMIT);
    right_Controller.SetOutputLimits(-linY_LIMIT,linY_LIMIT);
    left_Controller.Compute();
    right_Controller.Compute();
    left_Controller.SetMode(AUTOMATIC);
    right_Controller.SetMode(AUTOMATIC);
    
}

// turn to angle. 
void Drivetrain::TurnToAngle(double theta)
{
    driveMode = 2;
    
    theta_Setpoint = theta;

    theta_Controller.SetTunings(angZ_TURN_kP, angZ_TURN_kI, angZ_TURN_kD);
    theta_Controller.SetOutputLimits(-angZ_TURN_LIMIT,angZ_TURN_LIMIT);
    theta_Controller.SetMode(AUTOMATIC);
}

// follow an arc length on a defined circle, with turning direction specified
void Drivetrain::ArcLengthDrive(double radius, double theta, bool right)
{
    driveMode = 3;
}