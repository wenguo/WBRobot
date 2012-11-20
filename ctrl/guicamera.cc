#include <common/common.hh>
#include <gazebo.hh>

gazebo::transport::NodePtr guiNode, guicameraNode;
gazebo::transport::PublisherPtr guicameraPub;
gazebo::transport::SubscriberPtr guiSub;
gazebo::common::Time updateTimestamp;

void OnUpdate(ConstGUIPtr &_msg);

int main(int argc, char**argv)
{

    gazebo::transport::init();
    gazebo::transport::run();

    std::string worldName = "default";
    guicameraNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    guicameraNode->Init(worldName);
    guicameraPub = guicameraNode->Advertise<gazebo::msgs::GUI>(std::string("~/") +
            "gui");

    guiNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    guiNode->Init(worldName);
    guiSub = guiNode->Subscribe(std::string("~/") +
            "gui", &OnUpdate, NULL);
    double x=0, r=0;
    while(1)
    {
            x +=0.2;
            r +=0.1;
            gazebo::msgs::GUI msg;
            gazebo::msgs::GUICamera *cam = msg.mutable_camera();
            cam->set_name("user_cam2");
            gazebo::msgs::Set(cam->mutable_origin(),
                    gazebo::math::Pose(x, 0, 0, r, 0, 0));
            guicameraPub->Publish(msg);

        usleep(1000000);
    }
    return 0;
}

void OnUpdate(ConstGUIPtr &_msg)
{
    static double datatime=0;
    static int count = 0;

   // if (gazebo::common::Time::GetWallTime() - updateTimestamp > gazebo::common::Time(1,10000000))
    {

        updateTimestamp = gazebo::common::Time::GetWallTime();

        {
            const gazebo::msgs::GUICamera &camera = _msg->camera();
            std::cout<<camera.name()<<std::endl;
            std::cout<<camera.origin().position().x()<<" "
                     <<camera.origin().position().y()<<" "
                     <<camera.origin().position().z()<<std::endl;

        }
    }
    printf("On GUI Update\n");
}
