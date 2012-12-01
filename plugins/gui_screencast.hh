#include <gazebo.hh>
#include <gui/Gui.hh>
#include <rendering/UserCamera.hh>
#include <rendering/Scene.hh>

namespace gazebo
{
    class GUIScreenCast : public SystemPlugin
    {
        public: 
            GUIScreenCast();
            void Load(int _argc, char ** _argv);

            void Init();

            void OnNewFrame(const unsigned char *_image,
                              unsigned int _width, unsigned int _height,
                              unsigned int _depth, const std::string &_format);

            void Render();
            void PostRender();
        private: 
            unsigned int width, height, depth;
            std::string format;
            std::vector<event::ConnectionPtr> connections;
            rendering::UserCameraPtr userCam;
            rendering::CameraPtr camera;
            int saveCount;
    };

}
