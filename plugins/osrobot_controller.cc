#include "osrobot_controller.hh"

using namespace gazebo;

OSRobotController::OSRobotController()
{
    this->forwardSpeed = 0;
    this->turnAngle = 0;

}

OSRobotController::~OSRobotController()
{
}

void OSRobotController::Init()
{
}

void OSRobotController::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    this->model = _parent;
    //this->connections.push_back(event::Events::ConnectWorldUpdateStart(
    //            boost::bind(&OSRobotController::OnUpdate, this)));
    this->forwardSpeed = _sdf->GetValueDouble("forward_speed");
    this->turnAngle = _sdf->GetValueDouble("turn_angle");



    this->velNode = transport::NodePtr(new transport::Node());
    this->velNode->Init(this->model->GetWorld()->GetName());
    std::cout<<"Load world: "<<this->model->GetWorld()->GetName()<<std::endl;
    this->velPub = this->velNode->Advertise<gazebo::msgs::Pose>(std::string("~/") +
            this->model->GetName() + "/vel_cmd");


    this->laserNode = transport::NodePtr(new transport::Node());
    this->laserNode->Init(this->model->GetWorld()->GetName());
    this->laserSub = this->laserNode->Subscribe(std::string("~/") +
            this->model->GetName() + "/chassis/IR1/scan", &OSRobotController::OnScan, this);

}

void OSRobotController::OnUpdate()
{

}

void OSRobotController::OnScan(ConstLaserScanPtr &_msg)
{
    static double datatime=0;
    static int count = 0;

    if (gazebo::common::Time::GetWallTime() - this->updateTimestamp > common::Time(1,10000000))
    {

        this->updateTimestamp = gazebo::common::Time::GetWallTime();

        {
            this->turnAngle = count++ % 60 - 30;
            gazebo::msgs::Pose msg;
            gazebo::msgs::Set(msg.mutable_position(),
                    gazebo::math::Vector3(this->forwardSpeed, this->turnAngle, 0));
            gazebo::msgs::Set(msg.mutable_orientation(),
                    gazebo::math::Quaternion(0, 0, 0));
            this->velPub->Publish(msg);
        }
    }

}


// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(OSRobotController)
