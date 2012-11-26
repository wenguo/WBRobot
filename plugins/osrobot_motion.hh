#include <common/common.hh>
#include <physics/physics.hh>
#include <sensors/Sensors.hh>
#include <sensors/RaySensor.hh>
#include <gazebo.hh>

#include "req.pb.h"
typedef const boost::shared_ptr<my_msgs::Req const> ConstReqPtr;

namespace gazebo
{
    enum {RIGHT, LEFT};
    
    class OSRobot : public ModelPlugin
    {
        public:
            OSRobot();
            ~OSRobot();
            virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
            virtual void Init();

            void OnUpdate();

            void OnVelMsg(ConstPosePtr &_msg);
            void OnReqMsg(ConstReqPtr &_msg);


        private:
            physics::ModelPtr model;

            // Pointer to the update event connection
            event::ConnectionPtr updateConnection;

            common::Time updateTimestamp;
            
            std::vector<event::ConnectionPtr> connections;
            physics::LinkPtr chassis;
            std::vector<physics::JointPtr> joints;
            math::Vector3 velocity;
            math::Pose pose;

            double forwardSpeed;
            double turnAngle;
            double wheelSeparation;
            double frontPower, rearPower;
            double maxSpeed;
            double wheelRadius;

            double aeroLoad;
            double swayForce;

            bool following_camera;


            transport::NodePtr node;
            transport::SubscriberPtr velSub;
            transport::SubscriberPtr reqSub;

    };
}
