////////////////////////////////////////////////////////////////////// Drivetrain //////////////////////////////////////////////////////////////////////

#define leftMotorChannel 4
#define rightMotorChannel 6
#define leftEncA 34
#define leftEncB 35
#define rightEncA 36
#define rightEncB 39


#define angZ_TURN_kS 0.6
#define angZ_TURN_kP 0.1
#define angZ_TURN_kI 0.0
#define angZ_TURN_kD 0.05

#define angZ_TURN_LIMIT 0.4

// in degrees
#define theta_ErrorThreshold 2

#define linY_kS 0.8

#define linY_kP 0.1
#define linY_kI 0.0
#define linY_kD 0.0

#define linY_LIMIT 0.5

// in mm
#define y_ErrorThreshold 5

// diamater * pi / ticks per rev
// divide to convert mm -> tick
// multiply to convert tick -> mm
#define mmPerTick 200/711.2


////////////////////////////////////////////////////////////////////// Shooter //////////////////////////////////////////////////////////////////////

#define topMotorChannel 2
#define bottomMotorChannel 3

#define shooterDelay 10

#define AMP_Top_kS 0.8
#define AMP_Bottom_kS 0.8
// #define AMP_Top_kS 0.75
// #define AMP_Bottom_kS 0.64


#define SUBWOOFER_Top_kS 1.0
#define SUBWOOFER_Bottom_kS 1.0

#define PODIUM_Top_kS 0.0
#define PODIUM_Bottom_kS 0.0

#define PASS_Top_kS 1.0
#define PASS_Bottom_kS 1.0

#define INTAKE_Top_kS -1.0
#define INTAKE_Bottom_kS -1.0

////////////////////////////////////////////////////////////////////// Indexer //////////////////////////////////////////////////////////////////////

#define intakeMotorChannel 5

#define Intake_IN_kS 1.0
#define Intake_SHOOT_kS 1.0

#define pinSensor 4
#define feedbackLED 5

#define Distance_EMPTY 10
#define Distance_NOTE 

////////////////////////////////////////////////////////////////////// Climber //////////////////////////////////////////////////////////////////////

#define climberChannel 1
#define reactionClimberChannel 2

#define climberStowAngle   0
#define climberDeployAngle 105

#define reactionStowAngle   105
#define reactionDeployAngle 0

////////////////////////////////////////////////////////////////////// Controller //////////////////////////////////////////////////////////////////////

#define deadzoneValue 0.05

#define axisLinY 1
#define axisAngZ 0

#define buttonIntake 6 
#define buttonExecute 7

#define buttonAmp 1
#define buttonPass 2
#define buttonSub 0
#define buttonPodium 3
#define buttonSource 4

#define buttonClimb 5
#define buttonDeployReaction 15
#define buttonStowReaction 14

#define buttonZeroYaw 9

////////////////////////////////////////////////////////////////////// Autonomus //////////////////////////////////////////////////////////////////////

#define taxiDistanceIn 48