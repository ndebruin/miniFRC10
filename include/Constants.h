////////////////////////////////////////////////////////////////////// Drivetrain //////////////////////////////////////////////////////////////////////

#define leftMotorChannel 4
#define rightMotorChannel 2
#define leftEncA 34
#define leftEncB 35
#define rightEncA 36
#define rightEncB 39


#define angZ_TURN_kS 0.6
#define angZ_TURN_kP 0.001
#define angZ_TURN_kI 0.00
#define angZ_TURN_kD 0.005

#define angZ_TURN_LIMIT 0.3

#define angZ_LINEAR_kS 0
#define angZ_LINEAR_kP 0
#define angZ_LINEAR_kI 0
#define angZ_LINEAR_kD 0

#define angZ_LINEAR_LIMIT 0.4

// in degrees
#define theta_ErrorThreshold 0.5

#define linY_kS 0.0

#define linY_kP 0.0
#define linY_kI 0.0
#define linY_kD 0.0

// in mm
#define y_ErrorThreshold 0.5 * mmPerTick

// diamater * pi / ticks per rev
// divide to convert mm -> tick
// multiply to convert tick -> mm
#define mmPerTick 67.0 * 3.14159 / 40.0


////////////////////////////////////////////////////////////////////// Shooter //////////////////////////////////////////////////////////////////////

#define topMotorChannel 3
#define bottomMotorChannel 5

#define shooterDelay 0.1

#define AMP_Top_kS 0.0
#define AMP_Bottom_kS 0.0

#define SUBWOOFER_Top_kS 0.0
#define SUBWOOFER_Bottom_kS 0.0

#define PODIUM_Top_kS 0.0
#define PODIUM_Bottom_kS 0.0

#define PASS_Top_kS 0.0
#define PASS_Bottom_kS 0.0

#define INTAKE_Top_kS 0.0
#define INTAKE_Bottom_kS 0.0

////////////////////////////////////////////////////////////////////// Indexer //////////////////////////////////////////////////////////////////////

#define intakeMotorChannel 1

#define Intake_IN_kS 0.9
#define Intake_SHOOT_kS 0.8

#define Distance_EMPTY 20
#define Distance_NOTE 10

////////////////////////////////////////////////////////////////////// Climber //////////////////////////////////////////////////////////////////////

#define leftClimberChannel 1
#define rightClimberChannel 2

#define leftStowAngle   150
#define leftDeployAngle 15

#define rightStowAngle   15
#define rightDeployAngle 150