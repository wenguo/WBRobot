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
}

void OSRobotVisual::Load(rendering::VisualPtr _visual, sdf::ElementPtr _sdf)
{
    printf("Visual plugin loaded\n");
    std::cout<<_visual->GetName()<<std::endl;
    
    this->visual = _visual;

    if(this->visual->GetScene()->GetUserCameraCount() > 0)
        this->cam = this->visual->GetScene()->GetUserCamera(0);

    this->connections.push_back(event::Events::ConnectRender(
                boost::bind(&OSRobotVisual::OnUpdate, this)));


    this->node = transport::NodePtr(new transport::Node());
    this->node->Init("default");

    this->reqSub = this->node->Subscribe(std::string("~/") +
             + "/following_camera_req", &OSRobotVisual::OnReqMsg, this);


}

void OSRobotVisual::OnUpdate()
{
    math::Pose cam_pose = math::Pose(this->visual->GetWorldPose().pos + 
            math::Vector3(3,-2,1), math::Quaternion(0, GZ_DTOR(11.31), GZ_DTOR(135)));

    if(this->cam)
    {
        this->cam->SetWorldPose(cam_pose);
    }

}

void OSRobotVisual::OnReqMsg(ConstReqPtr &_msg)
{
    printf("Set following camera for %s\n", _msg->name().c_str());

    this->following_camera = _msg->following_cam();
}

// Register this plugin with the simulator
GZ_REGISTER_VISUAL_PLUGIN(OSRobotVisual)
