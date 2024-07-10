#include <Arduino.h>
#include <BluetoothSerial.h>
#include <AlfredoConnect.h>
#include <Alfredo_NoU2.h>
#include <ESP32Encoder.h>
#include <Wire.h>
#include <ICM_20948.h>
#include <VL53L0X.h>
#include <PID_v1.h>

#include "IMU.h"
#include "Drivetrain.h"
#include "Intake.h"
#include "Shooter.h"

#include "Constants.h"

////////////////////////////////////////////////////////////////////// Function Declarations //////////////////////////////////////////////////////////////////////

// function def




////////////////////////////////////////////////////////////////////// Hardware Declarations //////////////////////////////////////////////////////////////////////

// define bluetooth serial connection
BluetoothSerial serialBluetooth;
String robotName = "Team 43";

// define motors
NoU_Motor leftMotor(leftMotorChannel);
NoU_Motor rightMotor(rightMotorChannel);

// define IMU
IMU imu = IMU(21, 22);

// define drivetrain
Drivetrain drivetrain = Drivetrain(&leftMotor, &rightMotor, &imu);

NoU_Motor intakeMotor(intakeMotorChannel);

//Intake intake = Intake(&intakeMotor);

NoU_Motor topMotor(topMotorChannel);
NoU_Motor bottomMotor(bottomMotorChannel);

// Shooter shooter = Shooter(&topMotor, &bottomMotor);

NoU_Servo testServo(1);


////////////////////////////////////////////////////////////////////// Logic Declarations //////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////// setup() //////////////////////////////////////////////////////////////////////
void setup() {
  // begin DS comms
  serialBluetooth.begin(robotName);
  AlfredoConnect.begin(serialBluetooth); // providing true means we won't get annoying errors regarding lack of joystick data

  // start RSL
  pinMode(LED_BUILTIN, OUTPUT);

  // start subsystems
  uint8_t error_Drivetrain = drivetrain.begin();
  //uint8_t error_Intake = intake.begin();
  // uint8_t error_Shooter = shooter.begin()

  topMotor.setInverted(false);
  bottomMotor.setInverted(true);

}

double top = 0;
double bottom = 0;
////////////////////////////////////////////////////////////////////// loop() //////////////////////////////////////////////////////////////////////
void loop() {
  // parse updates from driver station
  AlfredoConnect.update();

  // update all subsystems
  int8_t error_IMU = drivetrain.updateIMU();
  uint8_t error_Drivetrain = drivetrain.update();
  //uint8_t error_Intake = intake.update();
  // uint8_t error_Shooter = shooter.update();

  if(error_Drivetrain != 1){
    serialBluetooth.println("Theta Error: " + String(drivetrain.getThetaError()) + " AngZ_Out: " + String(drivetrain.getAngZOut()));
  }
  else{
    serialBluetooth.println("done");
  }

  //serialBluetooth.print("\033c");
  // serialBluetooth.println("    Y Error: " + String(drivetrain.getYError()));
  
  

  double throttle = 0;
  double rotation = 0;
  
  if (AlfredoConnect.keyHeld(Key::W)) {
        throttle = 0.9;
        top = 0.7;
        bottom = 0.9;
  } else if (AlfredoConnect.keyHeld(Key::S)) {
        throttle = -0.9;
        top = 0.0;
        bottom = 0.0;
  }
  if (AlfredoConnect.keyHeld(Key::A)) {
        rotation = -0.9;
        top = 0.6;
        bottom = 0.8;
  } else if (AlfredoConnect.keyHeld(Key::D)) {
        rotation = 0.9;
        top = 0.9;
        bottom = 0.6;
  }
  if(AlfredoConnect.keyHeld(Key::Space)){
    drivetrain.ArcadeDrive(throttle, rotation);
  }

  if(AlfredoConnect.keyHeld(Key::T)){
    drivetrain.TurnToAngle(90);
  }

  if(AlfredoConnect.keyHeld(Key::Enter)){
    drivetrain.cancelAuto();
  }

  topMotor.set(top);
  bottomMotor.set(bottom);



}
////////////////////////////////////////////////////////////////////// Function Code //////////////////////////////////////////////////////////////////////
 
// joystick deadzone
double deadzone(double deadZoneVal, double rawJoy){
  if(fabs(rawJoy) < deadZoneVal){
    return 0.0;
  }
  return rawJoy;
}

