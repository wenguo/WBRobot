#include <gazebo.hh>
#include <gui/Gui.hh>
#include <rendering/UserCamera.hh>

namespace gazebo
{
    class ScreenCast : public SystemPlugin
    {
        public: 
            ScreenCast();
            void Load(int _argc, char ** _argv);

            void Init();

            void OnNewFrame(const unsigned char *_image,
                              unsigned int _width, unsigned int _height,
                              unsigned int _depth, const std::string &_format);
        private: 
            unsigned int width, height, depth;
            std::string format;
            event::ConnectionPtr newFrameConnection;
            rendering::UserCameraPtr userCam;
            int saveCount;
    };

}
