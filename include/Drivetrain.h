#ifndef Drivetrain_h
#define Drivetrain_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Alfredo_NoU2.h>
#include <PID_v1.h>

#include "IMU.h"

#include <Constants.h>

class Drivetrain
{
    public:
        Drivetrain::Drivetrain(NoU_Motor* LeftMotor, NoU_Motor* RightMotor, IMU* IMU);
        uint8_t begin();
        uint8_t update();

        void ArcadeDrive(double linY, double AngZ);
        void ChezyDrive(double linY, double AngZ, boolean isQuickTurn);
        void LinearHeadingDrive(double linY, double theta);
        void TurnToAngle(double theta);
        void ArcLengthDrive(double radius, double theta, bool right);


        
        
        

    private:
        ESP32Encoder leftEncoder;
        ESP32Encoder rightEncoder;
        NoU_Motor* leftMotor;
        NoU_Motor* rightMotor;
        IMU* imu;        
        
        // 0 - open loop
        // 1 - linear drive with heading correction
        // 2 - turn to angle
        // 3 - curve drive along an arc
        uint8_t driveMode;

        // theta PID controller
        double theta_Setpoint;
        double current_Theta;
        double angZ_Out;
        PID theta_Controller{&current_Theta, &angZ_Out, &theta_Setpoint, angZ_kP, angZ_kI, angZ_kD, DIRECT};

        // chezy drive values
        float quickStopThreshold = 0.2;
        float quickStopAlpha = 0.1;
        float quickStopAccumulator;


};

#endif