#include <Arduino.h>

// Alfredo stuff
#include <BluetoothSerial.h>
#include <AlfredoConnect.h>
#include <Alfredo_NoU2.h>

// additional libraries
#include <ESP32Encoder.h>
#include <Wire.h>
#include <ICM_20948.h>
#include <VL53L0X.h>
#include <PID_v1.h>

// subsystems
#include "IMU.h"
#include "Drivetrain.h"
#include "Intake.h"
#include "Shooter.h"
#include "Climber.h"

#include "Constants.h"

////////////////////////////////////////////////////////////////////// Function Declarations //////////////////////////////////////////////////////////////////////

double deadzone(double rawJoy);

////////////////////////////////////////////////////////////////////// Hardware Declarations //////////////////////////////////////////////////////////////////////

// define bluetooth serial connection
BluetoothSerial serialBluetooth;
String robotName = "Team 43";

State robotState;

// define motors
NoU_Motor leftMotor(leftMotorChannel);
NoU_Motor rightMotor(rightMotorChannel);

// define IMU
IMU imu = IMU(21, 22);

// define drivetrain
Drivetrain drivetrain = Drivetrain(&leftMotor, &rightMotor, &imu, &robotState);

NoU_Motor intakeMotor(intakeMotorChannel);

VL53L0X tofSensor;

Intake intake = Intake(&intakeMotor, &tofSensor, &robotState);

NoU_Motor topMotor(topMotorChannel);
NoU_Motor bottomMotor(bottomMotorChannel);

Shooter shooter = Shooter(&topMotor, &bottomMotor, &robotState);

NoU_Servo leftClimber(leftClimberChannel);
NoU_Servo rightClimber(rightClimberChannel);

Climber climber = Climber(&leftClimber, &rightClimber, &robotState);


////////////////////////////////////////////////////////////////////// Variable Declarations //////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////// setup() //////////////////////////////////////////////////////////////////////
void setup() {
  // begin DS comms
  serialBluetooth.begin(robotName);
  AlfredoConnect.begin(serialBluetooth); // providing true means we won't get annoying errors regarding lack of joystick data

  // start RSL
  pinMode(LED_BUILTIN, OUTPUT);

  // start subsystems
  uint8_t error_Drivetrain = drivetrain.begin();
  uint8_t error_Intake = intake.begin();
  uint8_t error_Shooter = shooter.begin();
  uint8_t error_Climber = climber.begin();
}


////////////////////////////////////////////////////////////////////// loop() //////////////////////////////////////////////////////////////////////
void loop() {
  // parse updates from driver station
  AlfredoConnect.update();

  // update all subsystems
  int8_t error_IMU = drivetrain.updateIMU();
  uint8_t error_Drivetrain = drivetrain.update();
  uint8_t error_Intake = intake.update();
  uint8_t error_Shooter = shooter.update();
  uint8_t error_Climber = climber.update();

  if(error_Drivetrain != 1){
    serialBluetooth.println("Theta Error: " + String(drivetrain.getThetaError()) + " AngZ_Out: " + String(drivetrain.getAngZOut()));
  }
  else{
    serialBluetooth.println("done");
  }

  if(error_Shooter == 1){ // shot finished
    intake.stop();
  }

  //serialBluetooth.print("\033c");
  // serialBluetooth.println("    Y Error: " + String(drivetrain.getYError()));
  
  float linY = deadzone(AlfredoConnect.getAxis(0, axisLinY));
  float angZ = deadzone(AlfredoConnect.getAxis(0, axisAngZ));

  drivetrain.ArcadeDrive(linY, angZ);

  // run intake
  if(AlfredoConnect.buttonHeld(0, buttonIntake)){
    intake.runUntilBreak();
  }
  // ensure we aren't pulsating the intake while trying to shoot
  else if(!AlfredoConnect.buttonHeld(0, buttonIntake)
          && !AlfredoConnect.buttonHeld(0, buttonExecute))
  {
    intake.stop();
  }

  // queue system
  if(AlfredoConnect.buttonHeld(0, buttonAmp)){
    robotState.setNextShot(1);
  }
  else if(AlfredoConnect.buttonHeld(0, buttonSub)){
    robotState.setNextShot(2);
  }
  else if(AlfredoConnect.buttonHeld(0, buttonPodium)){
    robotState.setNextShot(3);
  }
  else if(AlfredoConnect.buttonHeld(0, buttonPass)){
    robotState.setNextShot(4);
  }
  else if(AlfredoConnect.buttonHeld(0, buttonSource)){
    robotState.setNextShot(5);
  }
  else{
    robotState.setNextShot(0);
  }

  // execute
  if(AlfredoConnect.buttonHeld(0, buttonExecute)){
    if(robotState.getNextShot() == 1){ // amp
      shooter.ampShot();
      delay(5);
      intake.runUntilEmpty();
    }
    else if(robotState.getNextShot() == 2){ // subwoofer
      shooter.subShot();
      delay(5);
      intake.runUntilEmpty();
    }
    else if(robotState.getNextShot() == 3){ // podium
      shooter.podiumShot();
      delay(5);
      intake.runUntilEmpty();
    }
    else if(robotState.getNextShot() == 4){ // pass
      shooter.passingShot();
      delay(5);
      intake.runUntilEmpty();
    }
  }

  if(AlfredoConnect.buttonHeld(0, buttonClimb)){
    climber.deploy();
  }
  else{
    climber.stow();
  }

  if(AlfredoConnect.keyHeld(Key::T)){
    drivetrain.TurnToAngle(90);
  }

  if(AlfredoConnect.keyHeld(Key::U)){
    drivetrain.LinearHeadingDrive(25.4*12);
  }

  // enable / disable
  if(AlfredoConnect.keyHeld(Key::Enter)){
    robotState.setEnable(!robotState.isEnabled());
    digitalWrite(LED_BUILTIN, robotState.isEnabled());
  }


}
////////////////////////////////////////////////////////////////////// Function Code //////////////////////////////////////////////////////////////////////
 
// joystick deadzone
double deadzone(double rawJoy){
  if(fabs(rawJoy) < deadzoneValue){
    return 0.0;
  }
  return rawJoy;
}

