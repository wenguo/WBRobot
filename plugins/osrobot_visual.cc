#include "osrobot_visual.hh"
using namespace gazebo;

OSRobotVisual::OSRobotVisual()
{

    printf("Visual plugin created\n");
}

OSRobotVisual::~OSRobotVisual()
{
}

void OSRobotVisual::Init()
{
    printf("Visual plugin inited\n");
}

void OSRobotVisual::Load(rendering::VisualPtr _visual, sdf::ElementPtr _sdf)
{
    printf("Visual plugin loaded\n");
    std::cout<<_visual->GetName()<<std::endl;
    this->connections.push_back(event::Events::ConnectRender(
                boost::bind(&OSRobotVisual::OnUpdate, this)));


    this->node = transport::NodePtr(new transport::Node());
    this->node->Init("default");

    this->reqSub = this->node->Subscribe(std::string("~/") +
             + "/following_camera_req", &OSRobotVisual::OnReqMsg, this);


}

void OSRobotVisual::OnUpdate()
{

    printf("Visual update\n");

}

void OSRobotVisual::OnReqMsg(ConstReqPtr &_msg)
{
    printf("Set following camera\n");

    this->following_camera = _msg->following_cam();
}

// Register this plugin with the simulator
GZ_REGISTER_VISUAL_PLUGIN(OSRobotVisual)
