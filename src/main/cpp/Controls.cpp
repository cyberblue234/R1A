#include "Controls.h"

bool noteInBot = false;

Controls::Controls(Drivetrain *swerve, Shooter *shooter, Intake *intake, Elevator *elevator, Feeder *feeder, Limelight *limelight3, ctre::phoenix6::Orchestra *orchestra)
{
    this->swerve = swerve;
    this->shooter = shooter;
    this->intake = intake;
    this->feeder = feeder;
    this->elevator = elevator;
    this->limelight3 = limelight3;
    this->orchestra = orchestra;
}

void Controls::Periodic()
{
    SetSelectedRotaryIndex(AnalogToRotaryIndex(controlBoard.GetX()));
    
    DriveControls();
    ShooterControls();
    IntakeControls();
    ElevatorControls();
    FeederControls();
}

void Controls::DriveControls()
{
    if (gamepad.GetXButton() == true)
        swerve->SetFieldRelative(true);
    if (gamepad.GetBButton() == true)
        swerve->SetFieldRelative(false);
    if (gamepad.GetYButton() == true)
    {
        swerve->ResetGyroAngle();
        if (limelight3->GetTargetValid() == 1)
            swerve->ResetPose(limelight3->GetRobotPose());
    }

    if (gamepad.GetRightBumper()) 
        swerve->SetPIDFs();

    if (swerve->IsAlignmentOn())
        swerve->AlignSwerveDrive();
    else if (controlBoard.GetRawButton(ControlBoardConstants::ANCHOR))
    {
        swerve->SetAnchorState();
        // orchestra->Play();
    }
    else
    {
        // if (orchestra->IsPlaying())
        //     orchestra->Pause();
        double rot = swerve->RotationControl(gamepad.GetRightX(), 
                                controlBoard.GetRawButton(ControlBoardConstants::SHOOT) 
                                && GetSelectedRotaryIndex() != ControlBoardConstants::MANUAL_SCORE);
        swerve->DriveWithInput(gamepad.GetLeftY(), gamepad.GetLeftX(), rot, gamepad.GetRightTriggerAxis() > 0.2);
    }
}

void Controls::ShooterControls()
{
    switch (GetSelectedRotaryIndex())
    {
        case ControlBoardConstants::POS_AMP_2:
            shooter->SetAmpRPM(2300);
            break;
        case ControlBoardConstants::POS_AMP_3:
            shooter->SetAmpRPM(2100);
            break;
        case ControlBoardConstants::POS_AMP_4:
            shooter->SetAmpRPM(2000);
            break;
        default:
            shooter->SetAmpRPM(2200);
            break;
    }
    if (controlBoard.GetRawButton(ControlBoardConstants::SHOOTER_MOTORS))
    {
        if (GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_MAIN
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_2
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_3
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_4)
            shooter->ShootAtAmp();
        else
            shooter->ShootAtSpeaker();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SOURCE_INTAKE))
        shooter->IntakeFromSource();
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        shooter->Purge();
    else
        shooter->SetShooterMotors(0.0);
}

void Controls::IntakeControls()
{
    if (controlBoard.GetRawButton(ControlBoardConstants::GROUND_INTAKE))
    {
        if (noteInBot == false && abs(elevator->GetShooterAngle() - elevator->GetIntakeAngle()) < 0.5)
            intake->IntakeFromGround();
        else
            intake->SetIntakeMotor(-0.1);
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        intake->Purge();
    else
        intake->SetIntakeMotor(0.0);
}

void Controls::ElevatorControls()
{
    if (gamepad.GetPOV() == 270) 
    {
        elevator->SetElevator1Motor(gamepad.GetRightBumper() ? -0.5 : 0.5);
        return;
    }
    else if (gamepad.GetPOV() == 90) 
    {
        elevator->SetElevator2Motor(gamepad.GetRightBumper() ? -0.5 : 0.5);
        return;
    }

    

    // Manual up - dpad up
    if (controlBoard.GetRawButton(ControlBoardConstants::ELEVATOR_UP))
    {
        if (elevator->GetElevator1TopLimit() == false && elevator->GetElevator1Encoder() < elevator->GetHardEncoderLimit())
            elevator->SetElevator1Motor(elevator->GetElevatorSpeed());
        else    
            elevator->SetElevator1Motor(0.0);
        if (elevator->GetElevator2TopLimit() == false && elevator->GetElevator2Encoder() < elevator->GetHardEncoderLimit())
            elevator->SetElevator2Motor(elevator->GetElevatorSpeed());
        else    
            elevator->SetElevator2Motor(0.0);
    }
    // Manual down - dpad down
    else if (controlBoard.GetRawButton(ControlBoardConstants::ELEVATOR_DOWN))
    {
        if (elevator->GetElevator1BottomLimit() == false)
            elevator->SetElevator1Motor(-elevator->GetElevatorSpeed());
        else    
            elevator->SetElevator1Motor(0.0);
        if (elevator->GetElevator2BottomLimit() == false)
            elevator->SetElevator2Motor(-elevator->GetElevatorSpeed());
        else    
            elevator->SetElevator2Motor(0.0);
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::GROUND_INTAKE))
    {
        elevator->ElevatorControl(elevator->GetIntakeAngle());
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SOURCE_INTAKE))
    {
        elevator->ElevatorControl(elevator->GetIntakeAngle());
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SHOOT) && GetSelectedRotaryIndex() != ControlBoardConstants::MANUAL_SCORE)
    {
    
        double angle;
        switch (GetSelectedRotaryIndex())
        {
            case ControlBoardConstants::AUTO_SCORE:
                if (limelight3->GetTargetValid() == 1)
                    angle = elevator->CalculateSpeakerAngle();
                else
                    angle = elevator->GetIntakeAngle();
                break;
            case ControlBoardConstants::POS_MID:
                angle = elevator->GetMidAngle();
                break;
            case ControlBoardConstants::POS_STAGE:
                angle = elevator->GetStageAngle();
                break;
            case ControlBoardConstants::POS_AMP_MAIN:
                angle = elevator->GetAmpAngle();
                break;
            case ControlBoardConstants::POS_AMP_2:
                angle = elevator->GetAmpAngle();
                break;
            case ControlBoardConstants::POS_AMP_3:
                angle = elevator->GetAmpAngle();
                break;
            case ControlBoardConstants::POS_AMP_4:
                angle = elevator->GetAmpAngle();
                break;
            // Default is the close angle
            default:
                angle = elevator->GetCloseAngle();
        }
        frc::SmartDashboard::PutNumber("Desired Elevator Angle", angle);
        elevator->ElevatorControl(angle);
    }
    else
        elevator->SetElevatorMotors(0.0);
}

void Controls::FeederControls()
{
    if (controlBoard.GetRawButton(ControlBoardConstants::SHOOT))
    {
        if (GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_MAIN
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_2
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_3
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_4)
            feeder->ShootAtAmp();
        else if (GetSelectedRotaryIndex() != ControlBoardConstants::MANUAL_SCORE)
        {
            bool swerveAlignment = abs(limelight3->GetAprilTagOffset()) < 5.0;
            bool elevatorAlignment = abs(elevator->GetShooterAngle() - elevator->CalculateSpeakerAngle()) < 0.5;
            frc::SmartDashboard::PutBoolean("Swerve Alignment", swerveAlignment);
            frc::SmartDashboard::PutBoolean("Elevator Alignment", elevatorAlignment);
            bool atAlignment = swerveAlignment && elevatorAlignment;
            if (shooter->GetShooter1RPM() >= shooter->GetSpeakerRPM() - 100 && atAlignment)
                feeder->ShootAtSpeaker();
        }
        else
            feeder->ShootAtSpeaker();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SOURCE_INTAKE))
    {
        if (feeder->IntakeFromSource()) RumbleGamepad();
        else StopRumble();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::GROUND_INTAKE))
    {
        if (feeder->IntakeFromGround()) 
        {
            RumbleGamepad();
            noteInBot = true;
        }
        else
        {
            StopRumble();
            noteInBot = false;
        }
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        feeder->Purge();
    else
    {
        feeder->SetFeedMotor(0.0);
        StopRumble();
    }
}