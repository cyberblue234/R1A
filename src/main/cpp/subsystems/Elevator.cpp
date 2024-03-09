#include "subsystems/Elevator.h"

Elevator::Elevator(Limelight *limelight3)
{
    this->limelight3 = limelight3;

    elevator1Motor.GetConfigurator().Apply(configs::TalonFXConfiguration{});
    configs::TalonFXConfiguration elevator1Config{};

    configs::MotorOutputConfigs elevator1MotorOutput{};
    elevator1MotorOutput.WithInverted(signals::InvertedValue::Clockwise_Positive);
    elevator1MotorOutput.WithNeutralMode(signals::NeutralModeValue::Brake);
    elevator1Config.WithMotorOutput(elevator1MotorOutput);

    elevator1Motor.GetConfigurator().Apply(elevator1Config);

    elevator2Motor.GetConfigurator().Apply(configs::TalonFXConfiguration{});
    configs::TalonFXConfiguration elevator2Config{};

    configs::MotorOutputConfigs elevator2MotorOutput{};
    elevator2MotorOutput.WithNeutralMode(signals::NeutralModeValue::Brake);
    elevator2Config.WithMotorOutput(elevator2MotorOutput);

    elevator2Motor.GetConfigurator().Apply(elevator2Config);

    shooterAngleEncoder.SetDistancePerRotation(-360);
}

void Elevator::Periodic()
{
    if (GetElevator1BottomLimit())
        ResetElevator1Encoder();
    if (GetElevator2BottomLimit())
        ResetElevator2Encoder();
    UpdateTelemetry();
}

// Returns the Angle from parallel to floor in degrees using limelight
double Elevator::CalculateSpeakerAngle()
{
    // Distance from speaker in meters
    auto distance = limelight3->GetDistanceFromTarget();
    if ((double) distance < 1.45) return -52;
    if ((double) distance < 1.6) return -51;
    if ((double) distance < 1.8) return -49;
    if ((double) distance < 2.0) return intakeAngle;
    // Height we are targetting in meters
    double targetHeight = ElevatorConstants::kSpeakerHeight + (((double)distance / ElevatorConstants::kForce) * ElevatorConstants::kGravity);
    // Get the angle we want to go to in order to shoot in radians, then converting it to degrees
    double targetAngle = atan(targetHeight / distance) * (180 / std::numbers::pi);
    // Adding the "Kickup" generated by the shooter and then returning that
    return -(targetAngle - ElevatorConstants::kKickup);
}

void Elevator::ElevatorControl(double angle)
{
    if (angle > GetShooterAngle()) 
    {
        if (GetElevator1TopLimit() == false && GetElevator1Encoder() < GetHardEncoderLimit())
            SetElevator1MotorPosition(angle);
        else
            SetElevator1Motor(0.0);
        if (GetElevator2TopLimit() == false && GetElevator2Encoder() < GetHardEncoderLimit())
            SetElevator2MotorPosition(angle);
        else
            SetElevator2Motor(0.0);
    }
    if (angle < GetShooterAngle()) 
    {
        if (GetElevator1BottomLimit() == false)
            SetElevator1MotorPosition(angle);
        else
            SetElevator1Motor(0.0);
        if (GetElevator2BottomLimit() == false)
            SetElevator2MotorPosition(angle);
        else
            SetElevator2Motor(0.0);
    }
}

void Elevator::UpdateTelemetry()
{
    frc::SmartDashboard::PutNumber("Shooter Angle Encoder Count", shooterAngleEncoder.GetAbsolutePosition());
    frc::SmartDashboard::PutNumber("Shooter Angle Degrees", GetShooterAngle()); 
    frc::SmartDashboard::PutNumber("Elevator 1 Encoder Pos", GetElevator1Encoder());
    frc::SmartDashboard::PutNumber("Elevator 2 Encoder Pos", GetElevator2Encoder());
    
}