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

using namespace gazebo;

GUIScreenCast::GUIScreenCast() : saveCount(0)
{
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

    this->camera->SetImageWidth(640);//this->userCam->GetImageWidth());
    this->camera->SetImageHeight(480);//this->userCam->GetImageHeight());
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


    this->connections.push_back(event::Events::ConnectRender(
                boost::bind(&GUIScreenCast::Render, this)));
    this->connections.push_back(event::Events::ConnectPostRender(
                boost::bind(&GUIScreenCast::PostRender, this)));
    this->connections.push_back(this->camera->ConnectNewImageFrame(
                boost::bind(&GUIScreenCast::OnNewFrame, this, _1, _2, _3, _4, _5)));
}

void GUIScreenCast::Render()
{
    this->camera->Render();
}

void GUIScreenCast::PostRender()
{
    this->camera->PostRender();
}

void GUIScreenCast::OnNewFrame(const unsigned char * _image,
        unsigned int _width,
        unsigned int _height,
        unsigned int _depth,
        const std::string &_format) 
{
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%04d.png",this->saveCount);

    if (this->saveCount < 100)
    {
        rendering::Camera::SaveFrame(_image, _width,
                _height, _depth, _format, tmp);
        // usleep(10000000);
        //  gzmsg << "Saving frame [" << this->saveCount
        //      << "] as [" << tmp << "]["<<this->width<<","<<this->height<<"]\n";
        this->saveCount++;
    }
}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(GUIScreenCast)
