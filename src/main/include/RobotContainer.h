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
#include "subsystems/Shooter.h"
#include "subsystems/Intake.h"
#include "subsystems/Feeder.h"
#include "Controls.h"

class RobotContainer
{
public:
	RobotContainer();

	frc2::CommandPtr GetAutonomousCommand();
	frc2::CommandPtr GetShootCommand();
	frc2::CommandPtr GetIntakeCommand();

	void OdometryInit()
	{
		
	};

	Drivetrain *GetSwerve() { return &swerve; };
	Shooter *GetShooter() { return &shooter; };
	Intake *GetIntake() { return &intake; };
	Feeder *GetFeeder() { return &feeder; };
	
	std::string GetAuto() { return autoChooser.GetSelected(); };

	void PlotAutonomousPath();

	void LogTeleopData();
	void LogAutoData();

private:
	Drivetrain swerve;
	Shooter shooter;
	Intake intake;
	Feeder feeder;

	frc::PowerDistribution pdp;

	Controls controls;

	frc::SendableChooser<std::string> autoChooser;

	FILE *t_output;
	frc::Timer logTimer;
};
