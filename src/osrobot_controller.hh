#include <common/common.hh>
#include <physics/physics.hh>
#include <sensors/Sensors.hh>
#include <sensors/RaySensor.hh>
#include <gazebo.hh>

namespace gazebo
{
    enum {RIGHT, LEFT};
    
    class OSRobotController : public ModelPlugin
    {
        public:
            OSRobotController();
            ~OSRobotController();
            virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
            virtual void Init();

            void OnUpdate();

            void OnVelMsg(ConstPosePtr &_msg);


        private:
            physics::ModelPtr model;

            // Pointer to the update event connection
            event::ConnectionPtr updateConnection;

            common::Time updateTimestamp;
            
            std::vector<event::ConnectionPtr> connections;
            transport::NodePtr node;
            transport::SubscriberPtr velSub;
    };
}
