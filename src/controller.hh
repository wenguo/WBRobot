#include <common/common.hh>
#include <physics/physics.h>
#include <sensors/Sensors.hh>
#include <sensors/RaySensor.hh>
#include <gazebo.hh>

namespace gazebo
{
    enum {RIGHT, LEFT};
    
    class WBRobot : public ModelPlugin
    {
        public:
            WBRobot();
            ~WBRobot();
            virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
            virtual void Init();

            void OnUpdate();

            void SetSpeed(double lspeed, double rspeed);

        private:
            physics::ModelPtr model;

            // Pointer to the update event connection
            event::ConnectionPtr updateConnection;

            common::Time updateTimestamp;
            
            
            physics::JointPtr leftJoint, rightJoint;
            physics::LinkPtr link, leftWheelLink, rightWheelLink;
            double wheelSpeed[2];
            double torque;
            double wheelSeparation;
            double wheelRadius;

            transport::NodePtr node;
            transport::SubscriberPtr laserSub;
            sensors::RaySensorPtr laser;
    };
}
