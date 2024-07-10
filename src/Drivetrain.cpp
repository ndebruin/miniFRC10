#include <Arduino.h>
#include "Drivetrain.h"

Drivetrain::Drivetrain(NoU_Motor* LeftMotor, NoU_Motor* RightMotor, IMU* IMU) 
                                    : leftMotor(LeftMotor), rightMotor(RightMotor), imu(IMU)
{ 
    //theta_Controller.SetSampleTime(50);
}

uint8_t Drivetrain::getMode(){
    return driveMode;
}

uint8_t Drivetrain::begin(){
    leftMotor->setInverted(false);
    rightMotor->setInverted(true);

    leftEncoder.attachHalfQuad(leftEncA, leftEncB);
    rightEncoder.attachHalfQuad(rightEncA, rightEncB);
    leftEncoder.setCount(0);
    rightEncoder.setCount(0);

    return imu->begin();
}
int8_t Drivetrain::updateIMU(){
    return imu->read();
}

// this should be run as fast as possible to allow for constant updates to yaw and control loops
uint8_t Drivetrain::update(){
    // update our yaw coordinate 
    current_Theta = imu->getYaw();

    // if we're in an open loop mode, we can't run the PID controllers.
    if(driveMode == 0){
        theta_Controller.SetMode(MANUAL);
    }

    // linear heading
    if(driveMode == 1){
        // find the median of the two encoders (to account for turning actions)
        // currentY = (leftEncoder.getCount() + rightEncoder.getCount()) / 2;

        if(abs(theta_Setpoint-current_Theta) < y_ErrorThreshold){
            cancelAuto();
            return 1;
        }

        theta_Controller.Compute();
        
        int direction = -1;
        if(current_Theta > theta_Setpoint){
            direction = 1;
        }

        angZ_powerOut = direction*(angZ_TURN_kS + abs(angZ_Out));

        ArcadeDrive(0, angZ_powerOut);

    }


    // turn to angle
    if(driveMode == 2){
        if(abs(theta_Setpoint-current_Theta) < theta_ErrorThreshold){
            cancelAuto();
            return 1;
        }

        theta_Controller.Compute();
        

        
        int direction = -1;
        if(current_Theta > theta_Setpoint){
            direction = 1;
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
            leftPower = maxInput;
            rightPower = linY + angZ;
        }
        else {
            leftPower = linY - angZ;
            rightPower = maxInput;
        }
    }
    leftMotor->set(leftPower);
    rightMotor->set(rightPower);
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

    leftMotor->set(leftPower);
    rightMotor->set(rightPower);
}

// drive in the current direction with heading correction 
// (this functionally overloads to theta = current_Theta)
void Drivetrain::LinearHeadingDrive(double mm){
    LinearHeadingDrive(mm, current_Theta);
}

// drive in a direction with heading correction
void Drivetrain::LinearHeadingDrive(double mm, double theta)
{
    driveMode = 1;

    resetEncoders();

    theta_Setpoint = theta;

    //ySetpoint = mm/mmPerTick;

    // setup heading correction control loop
    theta_Controller.SetTunings(angZ_LINEAR_kP, angZ_LINEAR_kI, angZ_LINEAR_kD);
    theta_Controller.SetOutputLimits(-angZ_LINEAR_LIMIT,angZ_LINEAR_LIMIT);
    theta_Controller.Compute();
    theta_Controller.SetMode(AUTOMATIC);

    // setup distance control loop
    /*y_Controller.SetTunings(linY_kP, linY_kI, linY_kD);
    y_Controller.SetOutputLimits(-1.0,1.0);
    y_Controller.Compute();
    y_Controller.SetMode(AUTOMATIC);*/
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