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
#include "Auton.h"

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

Auton auton = Auton(&drivetrain, &intake, &shooter, &robotState);

////////////////////////////////////////////////////////////////////// Variable Declarations //////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////// setup() //////////////////////////////////////////////////////////////////////
void setup() {
  // begin DS comms
  serialBluetooth.begin(robotName);
  AlfredoConnect.begin(serialBluetooth, true); // providing true means we won't get annoying errors regarding lack of joystick data

  Serial.begin(9600);

  // start RSL
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(pinSensor, INPUT);
  pinMode(feedbackLED, OUTPUT);



  // start subsystems
  uint8_t error_Drivetrain = drivetrain.begin();
  uint8_t error_Intake = intake.begin();
  uint8_t error_Shooter = shooter.begin();
  uint8_t error_Climber = climber.begin();
}


long leftCount = 0;
long rightCount = 0;
double yaw = 0;
bool firstRun = true;
bool hadNote = false;
////////////////////////////////////////////////////////////////////// loop() //////////////////////////////////////////////////////////////////////
void loop() {
  // parse updates from driver station
  AlfredoConnect.update();

  // update all subsystems
  uint8_t error_Drivetrain = drivetrain.update();
  uint8_t error_Intake = intake.update();
  uint8_t error_Shooter = shooter.update();
  uint8_t error_Auton = auton.update();


  // get updates from teensy
  if(Serial.available() > 0){
    leftCount = Serial.readStringUntil(',').toInt(); // left encoder
    rightCount = Serial.readStringUntil(',').toInt(); // right  encoder
    yaw = Serial.readStringUntil('\n').toDouble();
  }
  robotState.setEncoderCounts(leftCount, rightCount);
  robotState.setYaw(yaw);
  robotState.setNote(analogRead(pinSensor) > Distance_EMPTY);
  digitalWrite(feedbackLED, robotState.hasNote());

  // teleop controls
  if(!robotState.robotMode()){
    float linY = -deadzone(AlfredoConnect.getAxis(0, axisLinY));
    float angZ = -deadzone(AlfredoConnect.getAxis(0, axisAngZ));

    drivetrain.ArcadeDrive(linY, angZ);

    // run intake
    if(AlfredoConnect.buttonHeld(0, buttonIntake)){
      if(firstRun){
        intake.run();
        hadNote = robotState.hasNote();
        firstRun = false;
      }
      else{
        if(!hadNote && robotState.hasNote()){
          intake.stop();
        }
      }
    }
    else if(AlfredoConnect.buttonHeld(0, buttonSource)){
      shooter.intake();
      intake.outtake();
      firstRun = true;
    }
    else{
      intake.stop();
      firstRun = true;
    }

    // queue system
    if(AlfredoConnect.buttonHeld(0, buttonAmp)){
      robotState.setNextShot(1);
      serialBluetooth.println("AMP");
    }
    else if(AlfredoConnect.buttonHeld(0, buttonSub)){
      robotState.setNextShot(2);
      serialBluetooth.println("SPEAKER");
    }
    // else if(AlfredoConnect.buttonHeld(0, buttonPodium)){
    //   robotState.setNextShot(3);
    //   serialBluetooth.println("PODIUM");
    // }
    else if(AlfredoConnect.buttonHeld(0, buttonPass)){
      robotState.setNextShot(4);
      serialBluetooth.println("PASS");
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
  }

  // zero yaw
  if(AlfredoConnect.buttonHeld(0, buttonZeroYaw)){
    robotState.zeroYaw();
  }

  // testing auton
  // if(AlfredoConnect.keyHeld(Key::U)){
  //   intake.runUntilBreak();
  //   drivetrain.LinearHeadingDriveUntilIntake(-12*25.4);
  //   robotState.setAuto();
  // }

  // auton enable / disable code
  if(AlfredoConnect.keyHeld(Key::W)){
    auton.enableAuton();
    robotState.setAuto();
  }

  // state override if needed
  if(AlfredoConnect.keyHeld(Key::Q)){
    robotState.setTeleop();
    auton.disableAuton();
  }

  // enable / disable
  if(AlfredoConnect.keyHeld(Key::Space)){
    intake.stop();
    shooter.stop();
    drivetrain.cancelAuto();
    drivetrain.ArcadeDrive(0,0);
    robotState.setTeleop();
    auton.disableAuton();

    robotState.setEnable(false);
    digitalWrite(LED_BUILTIN, LOW);

  }

  if(AlfredoConnect.keyHeld(Key::ControlLeft)){
    robotState.setEnable(true);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // auton selection
  if(AlfredoConnect.keyHeld(Key::Digit1)){
    auton.taxi(-25.4*taxiDistanceIn);
    serialBluetooth.println("AUTON SELECTED: TAXI");
  }
  if(AlfredoConnect.keyHeld(Key::Digit2)){
    auton.preload();
    serialBluetooth.println("AUTON SELECTED: PRELOAD");
  }
  if(AlfredoConnect.keyHeld(Key::Digit3)){
    auton.preloadTaxi(-25.4*taxiDistanceIn);
    serialBluetooth.println("AUTON SELECTED: PRELOAD+TAXI");
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