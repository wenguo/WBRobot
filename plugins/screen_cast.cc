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
#include "screen_cast.hh"

using namespace gazebo;

ScreenCast::ScreenCast() : saveCount(0)
{
}

void ScreenCast::Load(int /*_argc*/, char ** /*_argv*/)
{
    // Don't forget to load the camera plugin
}

void ScreenCast::Init()
{
    this->userCam = gui::get_active_camera();

    this->userCam->EnableSaveFrame(true);
    this->userCam->SetSaveFramePathname("gazebo_frames");
    return;

    this->userCam->SetCaptureData(true);
    if(!this->userCam->GetRenderTexture())
        this->userCam->CreateRenderTexture("mytexture"); 
    this->newFrameConnection = this->userCam->ConnectNewImageFrame(
            boost::bind(&ScreenCast::OnNewFrame, this, _1, _2, _3, _4, _5));
    this->width = this->userCam->GetImageWidth();
    this->height = this->userCam->GetImageHeight();
    this->depth = this->userCam->GetImageDepth();
    this->format = this->userCam->GetImageFormat();

}

void ScreenCast::OnNewFrame(const unsigned char * _image,
                              unsigned int _width,
                              unsigned int _height,
                              unsigned int _depth,
                              const std::string &_format) 
{
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s-%04d.jpg",
            this->userCam->GetName().c_str(), this->saveCount);

    if (this->saveCount < 100)
    {
            rendering::Camera::SaveFrame(_image, this->width,
    this->height, this->depth, this->format, tmp);
           // usleep(10000000);
            gzmsg << "Saving frame [" << this->saveCount
                << "] as [" << tmp << "]["<<this->width<<","<<this->height<<"]\n";
            this->saveCount++;
    }
}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(ScreenCast)
