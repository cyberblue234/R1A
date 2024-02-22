#include "subsystems/Elevator.h"
#include "RobotExt.h"
#include <numbers>

Elevator::Elevator()
{
    elevatorMotor1.RestoreFactoryDefaults();
    elevatorMotor2.RestoreFactoryDefaults();

    elevatorMotor2.Follow(elevatorMotor1, true);

    frc::SmartDashboard::PutNumber("Elevator P", ElevatorConstants::kElevatorP);
    frc::SmartDashboard::PutNumber("Elevator I", ElevatorConstants::kElevatorI);
    frc::SmartDashboard::PutNumber("Elevator D", ElevatorConstants::kElevatorD);
    frc::SmartDashboard::PutNumber("Elevator F", ElevatorConstants::kElevatorF);
    //Set PIDF stuff
}

void Elevator::Periodic()
{
    elevatorPID.SetP(frc::SmartDashboard::GetNumber("Elevator P", ElevatorConstants::kElevatorP));
    elevatorPID.SetI(frc::SmartDashboard::GetNumber("Elevator I", ElevatorConstants::kElevatorI));
    elevatorPID.SetD(frc::SmartDashboard::GetNumber("Elevator D", ElevatorConstants::kElevatorD));
    elevatorPID.SetFF(frc::SmartDashboard::GetNumber("Elevator F", ElevatorConstants::kElevatorF));
}

/*
TODO: (feel free to ignore this, just an idea for a starting spot)

1. Way to control the motor manually
2. Encoder Counts to Degrees
3. PID(F) controller for smooth movement to an angle
4. Create the Elevator Object in robot
5. Dashboard updater/Periodic Control
*/

//Returns the Angle from parallel to floor in degrees using limelight
double Elevator::CalculateSpeakerAngle()
{
    //Distance from speaker in meters
    auto distance = limelight2.GetDistanceFromTarget();
    //Height we are targetting in meters
    double targetHeight = ElevatorConstants::kSpeakerHeight + (( (double) distance / ElevatorConstants::kForce) * ElevatorConstants::kGravity);
    //Get the angle we want to go to in order to shoot in radians, then converting it to degrees
    double targetAngle = atan(targetHeight / distance) * (180 / std::numbers::pi);
    //Adding the "Kickup" generated by the shooter and then returning that
    return (targetAngle - ElevatorConstants::kKickup);
}