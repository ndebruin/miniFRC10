#include <Arduino.h>
#include "Drivetrain.h"

Drivetrain::Drivetrain(NoU_Motor* LeftMotor, NoU_Motor* RightMotor, IMU* IMU) 
                                    : leftMotor(LeftMotor), rightMotor(RightMotor), imu(IMU)
{ 
    theta_Controller.SetSampleTime(50);
}


uint8_t Drivetrain::begin(){
    leftMotor->setInverted(false);
    rightMotor->setInverted(true);

    leftEncoder.attachHalfQuad(leftEncA, leftEncB);
    rightEncoder.attachHalfQuad(rightEncA, rightEncB);
    leftEncoder.setCount(0);
    rightEncoder.setCount(0);

}

// does nothing during open loop control
uint8_t Drivetrain::update(){
    current_Theta = imu->getYaw();
    if(driveMode == 0){
        theta_Controller.SetMode(MANUAL);
    }
    if(driveMode == 2){
        if(abs(theta_Setpoint-current_Theta) < theta_ErrorThreshold){
            driveMode = 0;
            return 1;
        }

        theta_Controller.Compute();

        double angZ_kS_dir = 1;
        if(theta_Setpoint < current_Theta){
            angZ_kS_dir = -1;
        }

        ArcadeDrive(0, angZ_Out + (angZ_kS * angZ_kS_dir));
    }
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

// drive in a direction with heading correction
void Drivetrain::LinearHeadingDrive(double linY, double theta)
{
    driveMode = 1;
}

// turn to angle. 
void Drivetrain::TurnToAngle(double theta)
{
    driveMode = 2;
    
    theta_Setpoint = theta;

    theta_Controller.SetOutputLimits(-1,1);
    theta_Controller.Compute();
    theta_Controller.SetMode(AUTOMATIC);
}

// follow an arc length on a defined circle, with turning direction specified
void Drivetrain::ArcLengthDrive(double radius, double theta, bool right)
{
    driveMode = 3;
}