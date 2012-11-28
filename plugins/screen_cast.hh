#include <common/common.hh>
#include <plugins/CameraPlugin.hh>
#include <gazebo.hh>

namespace gazebo
{
    class ScreenCast : public CameraPlugin
    {
        public: 
            ScreenCast();
            void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);

            void OnNewFrame(const unsigned char *_image,
                    unsigned int _width, unsigned int _height, unsigned int _depth,
                    const std::string &_format);

            void OnGUI(ConstGUIPtr &_msg);
        private: 
            int saveCount;
            transport::SubscriberPtr guiSub;
            transport::NodePtr node;

    };
}
