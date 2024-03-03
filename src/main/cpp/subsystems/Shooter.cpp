#include "subsystems/Shooter.h"

#define VORTEX_MAX_RPM 6784

Shooter::Shooter()
{
    shooter1Motor.RestoreFactoryDefaults();
    shooter2Motor.RestoreFactoryDefaults();

    frc::SmartDashboard::PutNumber("Shooter P", ShooterConstants::kShooterP);
    frc::SmartDashboard::PutNumber("Shooter I", ShooterConstants::kShooterI);
    frc::SmartDashboard::PutNumber("Shooter D", ShooterConstants::kShooterD);
    frc::SmartDashboard::PutNumber("Shooter F", ShooterConstants::kShooterF);

    shooter1Motor.SetInverted(true);

    frc::SmartDashboard::PutNumber("Shooter Speaker RPM", speakerRPM);
    frc::SmartDashboard::PutNumber("Shooter Amp Speed", ampSpeed);
    frc::SmartDashboard::PutNumber("Shooter Intake Speed", intakeSpeed);

    shooter1PID.SetP(ShooterConstants::kShooterP);
    shooter1PID.SetI(ShooterConstants::kShooterI);
    shooter1PID.SetD(ShooterConstants::kShooterD);
    shooter1PID.SetFF(ShooterConstants::kShooterF);

    shooter2PID.SetP(ShooterConstants::kShooterP);
    shooter2PID.SetI(ShooterConstants::kShooterI);
    shooter2PID.SetD(ShooterConstants::kShooterD);
    shooter2PID.SetFF(ShooterConstants::kShooterF);
}

void Shooter::Periodic()
{
    // shooter1PID.SetP(frc::SmartDashboard::GetNumber("Shooter P", ShooterConstants::kShooterP));
    // shooter1PID.SetI(frc::SmartDashboard::GetNumber("Shooter I", ShooterConstants::kShooterI));
    // shooter1PID.SetD(frc::SmartDashboard::GetNumber("Shooter D", ShooterConstants::kShooterD));
    // shooter1PID.SetFF(frc::SmartDashboard::GetNumber("Shooter F", ShooterConstants::kShooterF));

    // shooter2PID.SetP(frc::SmartDashboard::GetNumber("Shooter P", ShooterConstants::kShooterP));
    // shooter2PID.SetI(frc::SmartDashboard::GetNumber("Shooter I", ShooterConstants::kShooterI));
    // shooter2PID.SetD(frc::SmartDashboard::GetNumber("Shooter D", ShooterConstants::kShooterD));
    // shooter2PID.SetFF(frc::SmartDashboard::GetNumber("Shooter F", ShooterConstants::kShooterF));

    UpdateTelemetry();
}

void Shooter::ShootAtSpeaker()
{
    SetShooterMotorsRPM(speakerRPM);
}

void Shooter::ShootAtAmp()
{
    //ampSpeed = frc::SmartDashboard::GetNumber("Shooter Amp Speed", ampSpeed);
    SetShooterMotors(ampSpeed);
}

void Shooter::IntakeFromSource()
{
    intakeSpeed = frc::SmartDashboard::GetNumber("Shooter Intake Speed", intakeSpeed);
    SetShooterMotors(-intakeSpeed);
}

void Shooter::UpdateTelemetry()
{
    frc::SmartDashboard::PutNumber("Shooter1 RPM", shooter1Encoder.GetVelocity());
    frc::SmartDashboard::PutNumber("Shooter2 RPM", shooter2Encoder.GetVelocity());

    frc::SmartDashboard::PutNumber("Shooter1 Current", shooter1Motor.GetOutputCurrent());
    frc::SmartDashboard::PutNumber("Shooter2 Current", shooter2Motor.GetOutputCurrent());
}