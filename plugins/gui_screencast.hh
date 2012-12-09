#include <common/Time.hh>
#include <common/common.hh>
#include <rendering/UserCamera.hh>
#include <rendering/Scene.hh>
#include <gazebo.hh>
#include <gui/Gui.hh>

extern "C" {
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}


namespace gazebo
{
    class GUIScreenCast : public SystemPlugin
    {
        public: 
            GUIScreenCast();
            ~GUIScreenCast();
            void Load(int _argc, char ** _argv);

            void Init();

            void OnNewFrame(const unsigned char *_image,
                              unsigned int _width, unsigned int _height,
                              unsigned int _depth, const std::string &_format);

            void OnUpdate(ConstWorldStatisticsPtr &_msg);
            void Render();
            void PostRender();

            bool InitCodec(const char *filename);
            void EncodingVideo(const unsigned char * _image,  unsigned int _width, unsigned int _height,
                              unsigned int _depth);
            void FiniEncoding();
        private: 
            unsigned int width, height, depth;
            std::string format;
            std::vector<event::ConnectionPtr> connections;
            rendering::UserCameraPtr userCam;
            rendering::CameraPtr camera;

            transport::NodePtr node;
            transport::SubscriberPtr statsSub;
            common::Time simTime;
            common::Time lastUpdateTime;
            common::Time updatePeriod;
            bool update;
            int saveCount;

            AVCodec *codec;
            AVCodecContext *c;
            AVFrame *frame;
            AVPacket pkt;
            FILE *f;
            int frameCount;
            bool codecReady;
    };

}
