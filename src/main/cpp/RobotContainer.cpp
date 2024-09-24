#include "RobotContainer.h"

using namespace pathplanner;

RobotContainer::RobotContainer() : swerve(GetLimelight3()), limelight3("limelight"), limelight2("limelight-intake"),
								   pdp(1, frc::PowerDistribution::ModuleType::kRev),
								   controls(GetSwerve(), GetLimelight3(), GetIntake(), GetFeeder()) {}

frc2::CommandPtr RobotContainer::GetIntakeCommand()
{
	return frc2::RunCommand
	(
		[this]
		{
			this->GetIntake()->IntakeFromGround();
			this->GetFeeder()->IntakeFromGround();
		}
	).ToPtr().Until
	(
		[this]
		{
			return this->GetFeeder()->IsNoteSecured();
		}
	);
}