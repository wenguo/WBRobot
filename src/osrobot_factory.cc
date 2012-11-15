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
#include <physics/physics.hh>
#include <gazebo_core.hh>

namespace gazebo
{
  class Factory : public WorldPlugin
  {
      public: void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
      {
          int num_robots = 0;
              num_robots = _sdf->GetValueDouble("num_robots");

          for(int i=0;i<num_robots;i++)
          {
              sdf::SDFPtr modelSDF;
              modelSDF.reset(new sdf::SDF);  
              sdf::initFile("gazebo.sdf", modelSDF);
              sdf::readFile("models/osrobot/model.sdf", modelSDF);
              sdf::ElementPtr modelElem = modelSDF->root->GetElement("model");
              std::string modelName = modelElem->GetValueString("name");
              modelName = modelElem->GetValueString("name") + "_" +
                  boost::lexical_cast<std::string>(i); ;
              modelElem->GetAttribute("name")->Set(modelName);
              modelElem->GetElement("pose")->Set(
                      math::Pose(math::Vector3(i, 0, 0), math::Quaternion(0, 0, 0)));

              _parent->InsertModelSDF(*modelSDF);
          }
      }
  };

  // Register this plugin with the simulator
  GZ_REGISTER_WORLD_PLUGIN(Factory)
}
