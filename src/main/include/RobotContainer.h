#pragma once

#include <string>
#include <optional>

#include <frc2/command/CommandPtr.h>
#include <frc2/command/Commands.h>
#include <frc2/command/StartEndCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/WaitCommand.h>

#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/path/PathPlannerPath.h>
#include <pathplanner/lib/commands/PathPlannerAuto.h>
#include <pathplanner/lib/auto/NamedCommands.h>

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/PowerDistribution.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Limelight.h"
#include "subsystems/Feeder.h"
#include "subsystems/Intake.h"
#include "Controls.h"

class RobotContainer
{
public:
	RobotContainer();

	frc2::CommandPtr GetIntakeCommand();

	void OdometryInit()
	{
		limelight3.UpdateLimelightTracking();
		swerve.ResetPose(limelight3.GetRobotPose());
		limelight3.UpdateTelemetry();
	};

	Drivetrain *GetSwerve() { return &swerve; };
	Limelight *GetLimelight3() { return &limelight3; };
	Limelight *GetLimelight2() { return &limelight2; };
	Intake *GetIntake() { return &intake; };
	Feeder *GetFeeder() { return &feeder; };

private:
	Drivetrain swerve;
	Limelight limelight3;
	Limelight limelight2;
	Intake intake;
	Feeder feeder;

	frc::PowerDistribution pdp;

	Controls controls;

	FILE *t_output;
	frc::Timer logTimer;
};
