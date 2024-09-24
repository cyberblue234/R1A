#include "Robot.h"

void Robot::RobotInit()
{
	container.OdometryInit();

	for (int port = 5800; port <= 5805; port++)
	{
		wpi::PortForwarder::GetInstance().Add(port, "limelight.local", port);
	}
}

void Robot::RobotPeriodic()
{
	frc2::CommandScheduler::GetInstance().Run();
}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit()
{
	container.OdometryInit();
}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
	container.OdometryInit();
}

void Robot::TeleopPeriodic() {}

void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}

void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif
