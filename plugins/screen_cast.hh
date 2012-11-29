#include <common/common.hh>
#include <plugins/CameraPlugin.hh>
#include <rendering/UserCamera.hh>
#include <gazebo.hh>

namespace gazebo
{
    class ScreenCast : public CameraPlugin
    {
        public: 
            ScreenCast();
            void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);

            virtual void OnNewFrame(const unsigned char *_image,
                    unsigned int _width, unsigned int _height, unsigned int _depth,
                    const std::string &_format);

        private: 
            int saveCount;

    };
}
