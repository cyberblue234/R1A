#pragma (warning (push, 0))

#include "rev/CANSparkMax.h"
#include "Constants.h"
#include <frc/DutyCycleEncoder.h>
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix/motorcontrol/can/VictorSPX.h>

class Shooter
{
public:
    Shooter();
    void ShooterControl();
    void SetShooterMotor1(double power) { shooter1Motor.Set(power); };
    void SetShooterMotor2(double power) { shooter2Motor.Set(power); };
    void SetFeedMotor(double power) { feedMotor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, power); };
    double GetShooterAngle() { return shooterAngleEncoder.GetDistance(); };

private:
    rev::CANSparkMax shooter1Motor{RobotMap::SHOOTER_MOTOR1_ADDRESS, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax shooter2Motor{RobotMap::SHOOTER_MOTOR2_ADDRESS, rev::CANSparkMax::MotorType::kBrushless};
    rev::SparkRelativeEncoder shooter1Encoder = shooter1Motor.GetEncoder(rev::SparkRelativeEncoder::Type::kHallSensor);
    rev::SparkRelativeEncoder shooter2Encoder = shooter2Motor.GetEncoder(rev::SparkRelativeEncoder::Type::kHallSensor);
    rev::SparkPIDController shooter1PID = shooter1Motor.GetPIDController();
    rev::SparkPIDController shooter2PID = shooter2Motor.GetPIDController();

    frc::DutyCycleEncoder shooterAngleEncoder{1};

    ctre::phoenix::motorcontrol::can::VictorSPX feedMotor{RobotMap::SHOOTER_FEED_ADDRESS};

    double shooter1Power;
    double shooter2Power;
    double feedPower;
};