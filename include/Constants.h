////////////////////////////////////////////////////////////////////// Drivetrain //////////////////////////////////////////////////////////////////////

#define leftMotorChannel 2
#define rightMotorChannel 4
#define leftEncA 34
#define leftEncB 35
#define rightEncA 36
#define rightEncB 39


#define angZ_TURN_kS 0.7
#define angZ_TURN_kP 0.001
#define angZ_TURN_kI 0.00
#define angZ_TURN_kD 0.0005

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
#define y_ErrorThreshold 0.5

// diamater * pi / ticks per rev
// divide to convert mm -> tick
// multiply to convert tick -> mm
#define mmPerTick 67.0 * 3.14159 / 40.0


////////////////////////////////////////////////////////////////////// Shooter //////////////////////////////////////////////////////////////////////

#define topMotorChannel 
#define bottomMotorChannel

#define AMP_Top_kS 0
#define AMP_Bottom_kS 0

#define SUBWOOFER_Top_kS 0
#define SUBWOOFER_Bottom_kS 0

#define PODIUM_Top_kS 0
#define PODIUM_Bottom_kS 0

#define TRAP_Top_kS 0
#define TRAP_Bottom_kS 0

////////////////////////////////////////////////////////////////////// Indexer //////////////////////////////////////////////////////////////////////

#define intakeMotorChannel 1

#define Intake_IN_kS 0.8
#define Intake_SHOOT_kS 0.6

#define Distance_EMPTY 20
#define Distance_NOTE 

