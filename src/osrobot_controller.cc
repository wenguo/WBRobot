#include "osrobot_controller.hh"

using namespace gazebo;

OSRobotController::OSRobotController()
{

}

OSRobotController::~OSRobotController()
{
}

void OSRobotController::Init()
{
}

void OSRobotController::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    this->connections.push_back(event::Events::ConnectWorldUpdateStart(
                boost::bind(&OSRobotController::OnUpdate, this)));

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init(this->model->GetWorld()->GetName());

  this->velSub = this->node->Subscribe(std::string("~/") +
      this->model->GetName() + "/vel_cmd", &OSRobotController::OnVelMsg, this);
}

void OSRobotController::OnUpdate()
{
  
 }

void OSRobotController::OnVelMsg(ConstPosePtr &_msg)
{
}

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(OSRobotController)
