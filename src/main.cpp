#include <Arduino.h>

// Alfredo stuff
#include <BluetoothSerial.h>
#include <AlfredoConnect.h>
#include <Alfredo_NoU2.h>

// additional libraries
#include <PID_v1.h>

// subsystems
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

// define drivetrain
Drivetrain drivetrain = Drivetrain(&leftMotor, &rightMotor, &robotState);

NoU_Motor intakeMotor(intakeMotorChannel);

Intake intake = Intake(&intakeMotor, &robotState);

NoU_Motor topMotor(topMotorChannel);
NoU_Motor bottomMotor(bottomMotorChannel);

Shooter shooter = Shooter(&topMotor, &bottomMotor, &robotState);

NoU_Servo climberServo(climberChannel);
NoU_Servo reactionClimber(reactionClimberChannel);

Climber climber = Climber(&climberServo, &reactionClimber, &robotState);


////////////////////////////////////////////////////////////////////// Variable Declarations //////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////// setup() //////////////////////////////////////////////////////////////////////
void setup() {
  // begin DS comms
  serialBluetooth.begin(robotName);
  AlfredoConnect.begin(serialBluetooth, true); // providing true means we won't get annoying errors regarding lack of joystick data

  //Serial.begin(9600);

  // start RSL
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // start subsystems
  uint8_t error_Drivetrain = drivetrain.begin();
  uint8_t error_Intake = intake.begin();
  uint8_t error_Shooter = shooter.begin();
  uint8_t error_Climber = climber.begin();
}

int tof = 1000;
long leftCount = 0;
long rightCount = 0;
double yaw = 0;
////////////////////////////////////////////////////////////////////// loop() //////////////////////////////////////////////////////////////////////
void loop() {
  // parse updates from driver station
  AlfredoConnect.update();

  // update all subsystems
  uint8_t error_Drivetrain = drivetrain.update();
  uint8_t error_Intake = intake.update();
  uint8_t error_Shooter = shooter.update();


  // get updates from teensy
  // if(Serial.available() > 0){
  //   leftCount = Serial.readStringUntil(',').toInt(); // left encoder
  //   rightCount = Serial.readStringUntil(',').toInt(); // right  encoder
  //   tof = Serial.readStringUntil(',').toInt();
  //   yaw = Serial.readStringUntil('\n').toDouble();
  // }
  robotState.setEncoderCounts(leftCount, rightCount);
  robotState.setYaw(yaw);
  robotState.setNote(tof < Distance_EMPTY);
  
  
  float linY = -deadzone(AlfredoConnect.getAxis(0, axisLinY));
  float angZ = -deadzone(AlfredoConnect.getAxis(0, axisAngZ));

  drivetrain.ArcadeDrive(linY, angZ);

  // run intake
  if(AlfredoConnect.buttonHeld(0, buttonIntake)){
    intake.run();
  }
  else if(AlfredoConnect.buttonHeld(0, buttonSource)){
    shooter.intake();
    intake.outtake();
  }
  else{
    intake.stop();
  }

  // queue system
  if(AlfredoConnect.buttonHeld(0, buttonAmp)){
    robotState.setNextShot(1);
  }
  else if(AlfredoConnect.buttonHeld(0, buttonSub)){
    robotState.setNextShot(2);
  }
  // else if(AlfredoConnect.buttonHeld(0, buttonPodium)){
  //   robotState.setNextShot(3);
  // }
  else if(AlfredoConnect.buttonHeld(0, buttonPass)){
    robotState.setNextShot(4);
  }



  // execute
  if(AlfredoConnect.buttonHeld(0, buttonExecute)){
    if(robotState.getNextShot() == 1){ // amp
      shooter.ampShot();
    }
    else if(robotState.getNextShot() == 2){ // subwoofer
      shooter.subShot();
      
    }
    // else if(robotState.getNextShot() == 3){ // podium
    //   shooter.podiumShot();
    // }
    else if(robotState.getNextShot() == 4){ // pass
      shooter.passingShot();
    }
  }
  else if(!AlfredoConnect.buttonHeld(0, buttonSource)){
    shooter.stop();
  }

  if(AlfredoConnect.buttonHeld(0, buttonClimb)){
    climber.deployClimber();
  }
  else{
    climber.stowClimber();
  }

  if(AlfredoConnect.buttonHeld(0, buttonDeployReaction)){
    climber.deployReaction();
  }
  if(AlfredoConnect.buttonHeld(0, buttonStowReaction)){
    climber.stowReaction();
  }

  if(AlfredoConnect.keyHeld(Key::T)){
    drivetrain.TurnToAngle(30);
  }
  if(AlfredoConnect.keyHeld(Key::Y)){
    drivetrain.TurnToAngle(-30);
  }

  if(AlfredoConnect.buttonHeld(0, 9)){
    robotState.zeroYaw();
  }

  if(AlfredoConnect.keyHeld(Key::U)){
    drivetrain.LinearHeadingDrive(12*25.4);
  }

  // enable / disable
  if(AlfredoConnect.keyHeld(Key::E)){
    intake.stop();
    shooter.stop();
    drivetrain.ArcadeDrive(0,0);

    robotState.setEnable(!robotState.isEnabled());
    digitalWrite(LED_BUILTIN, robotState.isEnabled());

  }

  if(AlfredoConnect.keyHeld(Key::ControlLeft)){
    drivetrain.cancelAuto();
  }

  serialBluetooth.println("Next shot: " + String(robotState.getNextShot()) + " " + String(drivetrain.getThetaError()) + " " + String(tof) + " " + String(robotState.hasNote()) + " "+ String(drivetrain.getLeftError()) + " " + String(drivetrain.getRightError()));

}
////////////////////////////////////////////////////////////////////// Function Code //////////////////////////////////////////////////////////////////////
 
// joystick deadzone
double deadzone(double rawJoy){
  if(fabs(rawJoy) < deadzoneValue){
    return 0.0;
  }
  return rawJoy;
}