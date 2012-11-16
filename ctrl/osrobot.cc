#include <common/common.hh>
#include <gazebo.hh>

gazebo::transport::NodePtr velNode, laserNode;
gazebo::transport::PublisherPtr velPub;
gazebo::transport::SubscriberPtr laserSub;
gazebo::common::Time updateTimestamp;
double forwardSpeed = 0.1;
double turnAngle = 0;

void OnScan(ConstLaserScanPtr &_msg);

int main(int argc, char**argv)
{

    gazebo::transport::init();
    gazebo::transport::run();

    std::string worldName = "default";
    velNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    velNode->Init(worldName);
    velPub = velNode->Advertise<gazebo::msgs::Pose>(std::string("~/") +
            "osrobot_0" + "/vel_cmd");

    laserNode = gazebo::transport::NodePtr(new gazebo::transport::Node());
    laserNode->Init(worldName);
    laserSub = laserNode->Subscribe(std::string("~/") +
            "osrobot_0" + "/chassis/IR1/scan", &OnScan, NULL);
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
