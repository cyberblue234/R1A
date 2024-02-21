#include "rev/CANSparkMax.h"
#include "Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc/DutyCycleEncoder.h>
#include <frc/DigitalInput.h>

class Intake : frc2::SubsystemBase
{
public:
    Intake();
    void IntakeControl();
    void SetIntakeMotor(double power) { intake.Set(power); };
    frc2::CommandPtr GetIntakeCommand();

private:
    rev::CANSparkMax intake{RobotMap::INTAKE_MOTOR_ADDRESS, rev::CANSparkMax::MotorType::kBrushless};
    rev::SparkRelativeEncoder intakeEncoder = intake.GetEncoder(rev::SparkRelativeEncoder::Type::kHallSensor);
    
    double power;
};