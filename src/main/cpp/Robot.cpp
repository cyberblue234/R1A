#include "Robot.h"

#include <frc2/command/CommandScheduler.h>

#include <frc/XboxController.h>
#include <frc/Joystick.h>
#include <frc/PowerDistribution.h>

frc::PowerDistribution pdp{1, frc::PowerDistribution::ModuleType::kRev};
frc::XboxController gamePad{0};
frc::Joystick controls(1);

void Robot::RobotInit() {}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {
  autonomousCommand = container.GetAutonomousCommand();

  if (autonomousCommand) {
    autonomousCommand->Schedule();
  }
}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  if (autonomousCommand) {
    autonomousCommand->Cancel();
  }
}

void Robot::TeleopPeriodic() {
  container.RunTeleop();
}

void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}

void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
