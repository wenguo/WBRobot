#include <common/common.hh>
#include <gazebo.hh>

#include "req.pb.h"

gazebo::transport::NodePtr node;
gazebo::transport::PublisherPtr velPub, guicameraPub;
gazebo::transport::SubscriberPtr laserSub, poseSub;
gazebo::common::Time updateTimestamp;
double forwardSpeed = 0.1;
double turnAngle = 0;

void OnScan(ConstLaserScanPtr &_msg);
void OnPoseMsg(ConstPosePtr &_msg);

std::string robotName("osrobot_0");

int main(int argc, char**argv)
{

    gazebo::transport::init();
    gazebo::transport::run();

    std::string worldName = "default";
    node = gazebo::transport::NodePtr(new gazebo::transport::Node());
    node->Init(worldName);

    velPub = node->Advertise<gazebo::msgs::Pose>(std::string("~/") +
            robotName + "/vel_cmd");

    poseSub = node->Subscribe("~/pose/info",&OnPoseMsg, NULL);

    laserSub = node->Subscribe(std::string("~/") +
            robotName + "/chassis/IR1/scan", &OnScan, NULL);

    guicameraPub = node->Advertise<my_msgs::Req>(std::string("~/") + robotName +
            "/req");

    usleep(1000000);

    if(guicameraPub)
    {
        my_msgs::Req msg;
        msg.set_following_cam(true);
        guicameraPub->Publish(msg);
    }



    while(1)
    {

        usleep(1000000);
    }


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
}

void OnPoseMsg(ConstPosePtr &_msg)
{
    if (_msg->name() != robotName)
        return;

    double px = _msg->position().x();
    double py = _msg->position().y();
    /*
       gazebo::msgs::GUI msg;
       gazebo::msgs::GUICamera *cam = msg.mutable_camera();
       cam->set_name("user_cam");
       gazebo::msgs::Set(cam->mutable_origin(),
       gazebo::math::Pose( px+2, py-2, 1, 0, GZ_DTOR(11.31), GZ_DTOR(135)));
       guicameraPub->Publish(msg);
       */

}
