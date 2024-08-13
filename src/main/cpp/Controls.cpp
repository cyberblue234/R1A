#include "Controls.h"

Controls::Controls(Drivetrain *swerve, Shooter *shooter, Intake *intake, Feeder *feeder)
{
    this->swerve = swerve;
    this->shooter = shooter;
    this->intake = intake;
    this->feeder = feeder;
}

void Controls::Periodic()
{
    SetSelectedRotaryIndex(AnalogToRotaryIndex(controlBoard.GetX()));
    
    DriveControls();
    ShooterControls();
    IntakeControls();
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
    }

    if (swerve->IsAlignmentOn())
        swerve->AlignSwerveDrive();
    else
    {
        double rot = swerve->RotationControl(gamepad.GetRightX(), false);
        swerve->DriveWithInput(gamepad.GetLeftY(), gamepad.GetLeftX(), rot, gamepad.GetRightTriggerAxis() > 0.2);
    }
}

void Controls::ShooterControls()
{
    switch (GetSelectedRotaryIndex())
    {
        case ControlBoardConstants::POS_AMP_2:
            shooter->SetAmpRPM(1150);
            break;
        case ControlBoardConstants::POS_AMP_3:
            shooter->SetAmpRPM(1100);
            break;
        case ControlBoardConstants::POS_AMP_4:
            shooter->SetAmpRPM(1000);
            break;
        default:
            shooter->SetAmpRPM(1200);
            break;
    }
    if (controlBoard.GetRawButton(ControlBoardConstants::SHOOTER_MOTORS))
    {
        if (GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_MAIN
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_2
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_3
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_4
        || GetSelectedRotaryIndex() == ControlBoardConstants::MANUAL_AMP)
            shooter->ShootAtAmp();
        else if (GetSelectedRotaryIndex() == ControlBoardConstants::PASS)
            shooter->Pass();
        else
            shooter->ShootAtSpeaker();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SOURCE_INTAKE) && feeder->IsNoteSecured() == false)
        shooter->IntakeFromSource();
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        shooter->Purge();
    else
        shooter->StopMotors();
}

void Controls::IntakeControls()
{
    if (controlBoard.GetRawButton(ControlBoardConstants::GROUND_INTAKE))
    {
        if (feeder->IsNoteSecured() == false)
        {
            intake->IntakeFromGround();
            // if (candle->GetCurrentMethod() != candle->kElevatorUp) {
            //     candle->LEDControls(LED::ControlMethods::kIntaking);
            // }
        }
        else
        {
            intake->SetIntakeMotor(-0.1);
            // if (candle->GetCurrentMethod() != candle->kElevatorUp) {
            //     candle->LEDControls(LED::ControlMethods::kNoteSecured);
            // }
        }
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        intake->Purge();
    else
        intake->StopMotor();
}

void Controls::FeederControls()
{
    if (controlBoard.GetRawButton(ControlBoardConstants::SHOOT))
    {
        if (GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_MAIN
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_2
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_3
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_AMP_4
        || GetSelectedRotaryIndex() == ControlBoardConstants::MANUAL_AMP)
            feeder->ShootAtAmp();
        else if (GetSelectedRotaryIndex() == ControlBoardConstants::PASS
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_STAGE
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_TRAP)
        {
            bool rpmSet;
            if (GetSelectedRotaryIndex() == ControlBoardConstants::POS_TRAP)
                rpmSet = shooter->GetAverageRPM() >= shooter->GetTrapRPM() - 50;
            else
                rpmSet = shooter->GetShooter1RPM() >= shooter->GetSpeakerRPM() - 100;
            
            if (rpmSet)
                feeder->ShootAtSpeaker();
        }
        else if (GetSelectedRotaryIndex() == ControlBoardConstants::MANUAL_SCORE
        || GetSelectedRotaryIndex() == ControlBoardConstants::POS_MID)
            feeder->ShootAtSpeaker();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::SOURCE_INTAKE))
    {
        feeder->IntakeFromSource();
        if (feeder->IsNoteSecured() == true) RumbleGamepad();
        else StopRumble();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::GROUND_INTAKE))
    {
        feeder->IntakeFromGround();
        if (feeder->IsNoteSecured() == true) RumbleGamepad();
        else StopRumble();
    }
    else if (controlBoard.GetRawButton(ControlBoardConstants::PURGE))
        feeder->Purge();
    else
    {
        feeder->StopMotor();
        StopRumble();
    }
}