#ifndef OSROBOT_VISUAL_HH
#define OSROBOT_VISUAL_HH

#include <common/common.hh>
#include <rendering/Visual.hh>
#include <gazebo.hh>

#include "req.pb.h"
typedef const boost::shared_ptr<my_msgs::Req const> ConstReqPtr;

namespace gazebo
{
    class OSRobotVisual : public VisualPlugin
    {
        public:
            OSRobotVisual();
            ~OSRobotVisual();
            virtual void Load(rendering::VisualPtr _visual, sdf::ElementPtr _sdf);
            virtual void Init();

            void OnUpdate();

            void OnReqMsg(ConstReqPtr &_msg);


        private:

            // Pointer to the update event connection
            event::ConnectionPtr updateConnection;

            common::Time updateTimestamp;
            
            std::vector<event::ConnectionPtr> connections;
            math::Pose pose;

            bool following_camera;


            transport::NodePtr node;
            transport::SubscriberPtr reqSub;

    };
}

#endif
