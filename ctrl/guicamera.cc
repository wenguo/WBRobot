#include <common/common.hh>
#include <gazebo.hh>

gazebo::transport::NodePtr guiNode, guicameraNode;
gazebo::transport::PublisherPtr guicameraPub;
gazebo::transport::SubscriberPtr guiSub;
gazebo::common::Time updateTimestamp;

void OnCamera(ConstLaserScanPtr &_msg);

int main(int argc, char**argv)
{

    gazebo::transport::init();
    gazebo::transport::run();

    std::string worldName = "default";
    guicameraNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    guicameraNode->Init(worldName);
    guicameraPub = velNode->Advertise<gazebo::msgs::Pose>(std::string("~/") +
            "gui" + "/camera");

    guiNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    guiNode->Init(worldName);
    guiSub = laserNode->Subscribe(std::string("~/") +
            "gui", &OnScan, NULL);
    while(1)
        usleep(10000000);

    return 0;
}

void OnScan(ConstLaserScanPtr &_msg)
{
    static double datatime=0;
    static int count = 0;

    if (gazebo::common::Time::GetWallTime() - updateTimestamp > gazebo::common::Time(1,10000000))
    {

        updateTimestamp = gazebo::common::Time::GetWallTime();

        {
            turnAngle = count++ % 60 - 30;
            gazebo::msgs::Pose msg;
            gazebo::msgs::Set(msg.mutable_position(),
                    gazebo::math::Vector3(forwardSpeed, turnAngle, 0));
            gazebo::msgs::Set(msg.mutable_orientation(),
                    gazebo::math::Quaternion(0, 0, 0));
            velPub->Publish(msg);
        }
    }
    printf("OnScan\n");
}
