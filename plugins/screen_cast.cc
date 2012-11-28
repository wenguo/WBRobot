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

void ScreenCast::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
{
    // Don't forget to load the camera plugin
    CameraPlugin::Load(_parent, _sdf);
    printf("Load ScreenCast\n");

    std::string worldName = "default";
    this->node = transport::NodePtr(new transport::Node());
    this->node->Init(worldName);
    this->guiSub = this->node->Subscribe(std::string("~/gui"), &ScreenCast::OnGUI, this);


}

void ScreenCast::OnNewFrame(const unsigned char * _image,
        unsigned int _width,
        unsigned int _height,
        unsigned int _depth,
        const std::string &_format) 
{
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s-%04d.jpg",
            this->camera->GetName().c_str(), this->saveCount);

    /*if (this->saveCount < 10)
    {
        this->camera->SaveFrame(
                _image, _width, _height, _depth, _format, tmp);
        gzmsg << "Saving frame [" << this->saveCount
            << "] as [" << tmp << "]\n";
        this->saveCount++;
    }
    */
}


void ScreenCast::OnGUI(ConstGUIPtr &_msg)
{
    printf("received gui message\n");
}

// Register this plugin with the simulator
GZ_REGISTER_SENSOR_PLUGIN(ScreenCast)
