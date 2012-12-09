/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "gui_screencast.hh"
#include <transport/Node.hh>

using namespace gazebo;

GUIScreenCast::GUIScreenCast() : saveCount(0), frameCount(0), codecReady(false)
{
}

GUIScreenCast::~GUIScreenCast()
{
    this->FiniEncoding();
}

void GUIScreenCast::Load(int /*_argc*/, char ** /*_argv*/)
{
}

void GUIScreenCast::Init()
{
    this->userCam = gui::get_active_camera();

    this->camera = this->userCam->GetScene()->CreateCamera("screencast_cam", false);
    if (!this->camera)
    {
        gzerr << "Unable to create camera sensor[mono_camera]\n";
        return;
    }

    this->camera->Load();

    this->camera->SetImageWidth(800);//this->userCam->GetImageWidth());
    this->camera->SetImageHeight(600);//this->userCam->GetImageHeight());
    this->camera->SetHFOV(this->userCam->GetHFOV());

    std::cout<<"width: "<<this->camera->GetImageWidth()<<"height"
        <<this->camera->GetImageHeight()<<std::endl;

    // Do some sanity checks
    if (this->camera->GetImageWidth() == 0 ||
            this->camera->GetImageHeight() == 0)
    {
        gzthrow("image has zero size");
    }

    this->camera->Init();
    this->camera->SetClipDist(0.1, 100);
    this->camera->CreateRenderTexture("screencast_cam_RttTex");
    this->camera->SetWorldPose(this->userCam->GetWorldPose());

   this->camera->SetCaptureData(true);

   this->node = transport::NodePtr(new transport::Node());
   this->node->Init();
   this->statsSub = this->node->Subscribe(
           std::string("~/world_stats"), &GUIScreenCast::OnUpdate, this);

   this->connections.push_back(event::Events::ConnectRender(
               boost::bind(&GUIScreenCast::Render, this)));
   this->connections.push_back(event::Events::ConnectPostRender(
               boost::bind(&GUIScreenCast::PostRender, this)));
   this->connections.push_back(this->camera->ConnectNewImageFrame(
               boost::bind(&GUIScreenCast::OnNewFrame, this, _1, _2, _3, _4, _5)));

   codecReady = InitCodec("test.mpg");
   std::cout<<this->camera->GetImageFormat()<<std::endl;
   std::cout<<this->camera->GetImageDepth()<<std::endl;

}

void GUIScreenCast::OnUpdate(ConstWorldStatisticsPtr &_msg)
{
    this->simTime = msgs::Convert(_msg->sim_time());
    this->camera->SetWorldPose(this->userCam->GetWorldPose());
}

void GUIScreenCast::Render()
{
    if(this->simTime - this->lastUpdateTime > common::Time(0.0))//this->updatePeriod)
    {
        this->lastUpdateTime = this->simTime;
        this->update = true;
        this->camera->Render();
    }
}

void GUIScreenCast::PostRender()
{
    if(this->update)
    {
        this->update = false;
        this->camera->PostRender();
    }
}

void GUIScreenCast::OnNewFrame(const unsigned char * _image,
        unsigned int _width,
        unsigned int _height,
        unsigned int _depth,
        const std::string &_format) 
{
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%04d.png",this->saveCount);

    if (this->saveCount < 1000)
    {
       // rendering::Camera::SaveFrame(_image, _width,
       //         _height, _depth, _format, tmp);
        // usleep(10000000);
        //  gzmsg << "Saving frame [" << this->saveCount
        //      << "] as [" << tmp << "]["<<this->width<<","<<this->height<<"]\n";
        this->saveCount++;

        EncodingVideo(_image,  _width, _height, _depth);
    }
    else if(this->saveCount ==1000)
        FiniEncoding();
}

bool GUIScreenCast::InitCodec(const char *filename)
{
     /* register all the codecs */
    avcodec_register_all();

    int i, ret, x, y, got_output;

    printf("Encode video file %s\n", filename);

    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        return false;
    }

    c = avcodec_alloc_context3(codec);

      /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 800;
    c->height = 600;
    /* frames per second */
    c->time_base= (AVRational){1,25};
    c->gop_size = 10; /* emit one intra frame every ten frames */
    c->max_b_frames=1;
    c->pix_fmt = PIX_FMT_YUV420P;

        /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return false;
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        return false;
    }

    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return false;
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        return false;
    }

    return true;

}

void GUIScreenCast::EncodingVideo(const unsigned char * _image,  unsigned int _width, unsigned int _height,
                              unsigned int _depth)
{
    if(!codecReady)
        return;

    int got_output;
    av_init_packet(&pkt);
    pkt.data = NULL;    // packet data will be allocated by the encoder
    pkt.size = 0;

    memcpy(frame->data[0], _image, _width * _height * _depth);
        /*
       for(y=0;y<c->height;y++) {
       for(x=0;x<c->width;x++) {
       frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
       }
       }

       for(y=0;y<c->height/2;y++) {
       for(x=0;x<c->width/2;x++) {
       frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
       frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
       }
       }*/

    frame->pts = frameCount++;

    /* encode the image */
    int ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
    if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        return; 
    }

    if (got_output) {
        printf("Write frame %3d (size=%5d)\n", frameCount, pkt.size);
        fwrite(pkt.data, 1, pkt.size, f);
        av_free_packet(&pkt);
    }

}

void GUIScreenCast::FiniEncoding()
{

    printf("finalise\n");
    if(!codecReady)
        return;
    /* get the delayed frames */
    int got_output=0, i=0;
    for (got_output = 1; got_output; i++) {
        fflush(stdout);

        int ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            return;
        }

        if (got_output) {
            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }
    }

    /* add sequence end code to have a real mpeg file */

    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    avcodec_free_frame(&frame);

}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(GUIScreenCast)
