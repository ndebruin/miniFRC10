#ifndef Drivetrain_h
#define Drivetrain_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Alfredo_NoU2.h>
#include <PID_v1.h>

#include "State.h"

#include <Constants.h>

class Drivetrain
{
    public:
        Drivetrain(NoU_Motor* LeftMotor, NoU_Motor* RightMotor, State* state);
        uint8_t begin();
        uint8_t update();

        uint8_t getMode();

        double getAngZOut(){
            return angZ_powerOut;
        }

        double getThetaError(){
            return theta_Setpoint - current_Theta;
        }

        double getLeftError(){
            return ySetpoint - currentLeft;
        }
        double getRightError(){
            return ySetpoint - currentRight;
        }

        void ArcadeDrive(double linY, double AngZ);
        void ChezyDrive(double linY, double AngZ, boolean isQuickTurn);
        void LinearHeadingDrive(double mm);
        void LinearHeadingDriveUntilIntake(double mm);
        void TurnToAngle(double theta);
        void ArcLengthDrive(double radius, double theta, bool right);

        void cancelAuto(){
            driveMode = 0;
            ArcadeDrive(0,0);
        }


    private:
        NoU_Motor* leftMotor;
        NoU_Motor* rightMotor;
        State* robotState;    
        
        // 0 - open loop
        // 1 - linear drive with heading correction
        // 2 - turn to angle
        // 3 - curve drive along an arc
        // 4 - linear until intake
        uint8_t driveMode;      

        // theta PID controller
        PID theta_Controller{&current_Theta, &angZ_Out, &theta_Setpoint, angZ_TURN_kP, angZ_TURN_kI, angZ_TURN_kD, DIRECT};
        double theta_Setpoint = 0; // using (-180,180)
        double current_Theta = 0;  // using (-180,180)
        double angZ_Out = 0;       // using (-1,1)
        double angZ_powerOut;      // using (-1,1)


        double ySetpoint = 0;      // using encoder ticks
        double currentLeft = 0;    // using encoder ticks
        double currentRight = 0;    // using encoder ticks
        int direction = 0;     // using (-1,1)
        

        // chezy drive values
        float quickStopThreshold = 0.2;
        float quickStopAlpha = 0.1;
        float quickStopAccumulator;

        void resetEncoders(){
            currentLeft = 0;
            currentRight = 0;
            robotState->zeroEncoders();
        }
};

#endif