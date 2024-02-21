
#include "RobotExt.h"
#include "subsystems/Limelight.h"
#include "subsystems/Drivetrain.h"


//Constructors, Name is fed in for the NetworkTables name
Limelight::Limelight(std::string name)
{
    limelight_name = name;
}

Limelight::Limelight()
{
    limelight_name = "limelight";
}

//Grabs Data from Limelight NetworkTables
void Limelight::UpdateLimelightTracking()
{
    std::shared_ptr<nt::NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable(limelight_name);

    //Getting Single Numbers from Limelight NetworkTable

    target_x = table->GetNumber("tx", 0.0); //X in relation to crosshair in Limelight view
    target_y = table->GetNumber("ty", 0.0); //Y in relation to crosshair in Limelight view
    target_area = table->GetNumber("ta", 0.0); //Target Area
    target_valid = table->GetNumber("tv", 0.0); //Checks to see if there is a valid target. 0 = no, 1 = yes
    april_tag_id = table->GetNumber("tid", 0); //Gets the ID of the current targetted AprilTag
    active_pipe = table->GetNumber("getpipe", 0.0); //Gets the active pipeline ID
    pipeline_latency = table->GetNumber("tl", 0.0); //Gets Pipeline latency contribution
    capture_latency = table->GetNumber("cl", 0.0); //Gets Capture Pipeline Latency

    //Getting Arrays from Limelight NetworkTable (X,Y,Z,Roll,Pitch,Yaw)
    
    //FIELD-SPACE Pose
    botpose = table->GetNumberArray("botpose", std::vector<double>(6)); //Returns Array on Bot Pose in field space
    botpose_blue = table->GetNumberArray("botpose_wpiblue", std::vector<double>(6)); //Returns Array on Bot Pose in blue DS field space
    botpose_red = table->GetNumberArray("botpose_wpired", std::vector<double>(6)); //Returns Array on Bot Pose in red DS field space

    //ROBOT-SPACE Pose
    targetpose_robotspace = table->GetNumberArray("targetpose_robotspace", std::vector<double>(6)); //Returns Array on Target Pose in Robot space 
}


//Sets the Pipeline ID (Check Enums in Limelight.h)
void Limelight::SetPipelineID(PipelineID pid)
{
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("pipeline", int(pid));
}

//Sets LEDs (Check Enums in Limelight.h)
void Limelight::SetLEDMode(LEDMode m)
{
    int led_mode = m;
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode", led_mode);
}

//Sets Operation Mode (Check Enums in Limelight.h)
void Limelight::SetCamMode(CamMode m)
{
    int cam_mode = m;
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("camMode", cam_mode);
}


//Gets Distance from an AprilTag in Meters
//MAKE SURE THIS CAN SEE A TAG BEFORE CALLING
auto Limelight::GetDistanceFromTarget()
{
    //Grabs the distance to target on the x and z planes (forward/back, left/right)
    double xDist = targetpose_robotspace.at(0);
    double zDist = targetpose_robotspace.at(2);

    //Find hypotenuse (total distance) of x and z planes  
    auto distance = units::meter_t(sqrt((xDist*xDist)+(zDist*zDist)));

    //Print out Distance to dashboard for debugging
    // frc::SmartDashboard::PutNumber("TARGET_DISTANCE", (double) distance);

    //Return the total distance
    return distance;
}

double Limelight::GetAprilTagOffset()
{
    double offset = atan(targetpose_robotspace.at(0) / targetpose_robotspace.at(2));
    frc::SmartDashboard::PutNumber("Target Rotatation Offset", offset);
    offset *= 180/PI;
    return offset;
}

//Returns the Robot Pose in Field space (Blue Origin) as a Pose2d Object
//MAKE SURE THIS CAN SEE A TAG BEFORE CALLING
frc::Pose2d Limelight::GetRobotPose()
{
    auto x = units::meter_t(botpose_blue.at(0)); //Gets X field coord from X limelight coord
    auto y = units::meter_t(botpose_blue.at(1)); //Gets Y field coord from Z limelight coord (since 3d -> 2d)
    frc::Rotation2d rot = frc::Rotation2d(units::degree_t(botpose_blue.at(5))); //Gets Rotation estimate from Limelight
    return frc::Pose2d(x, y, rot); //Constructs and Returns the Pose 2d Object
}

//SmartDashboard Updater for Debugging Purposes
void Limelight::UpdateTelemetry()
{
    //April Tags
    frc::SmartDashboard::PutNumber("APRIL_TAG_ID", april_tag_id);
    frc::SmartDashboard::PutNumber("TARGET_VALID", target_valid);
    frc::SmartDashboard::PutNumber("TARGET_DISTANCE", (double) GetDistanceFromTarget());

    //Pose
    frc::SmartDashboard::PutNumber("LL_POSE_X", (double) GetRobotPose().X());
    frc::SmartDashboard::PutNumber("LL_POSE_Y", (double) GetRobotPose().Y());
    frc::SmartDashboard::PutNumber("LL_POSE_ROT", (double) GetRobotPose().Rotation().Degrees());

    //Pipelines
    frc::SmartDashboard::PutNumber("TOTAL_LATENCY", GetTotalLatency());
    frc::SmartDashboard::PutNumber("PIPE_ID", GetActivePipeline());

    //Tracking
    frc::SmartDashboard::PutNumber("TX", target_x);
    frc::SmartDashboard::PutNumber("TY", target_y);
}