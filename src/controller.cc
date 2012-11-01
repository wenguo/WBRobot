#include "controller.hh"

using namespace gazebo;

WBRobot::WBRobot()
{
    this->wheelSpeed[LEFT] = this->wheelSpeed[RIGHT] = 0;
}

WBRobot::~WBRobot()
{
}

void WBRobot::Init()
{
    this->wheelSeparation = this->leftJoint->GetAnchor(0).Distance(
            this->rightJoint->GetAnchor(0));

    physics::EntityPtr parent = boost::shared_dynamic_cast<physics::Entity>(
            this->leftJoint->GetChild());

    math::Box bb = parent->GetBoundingBox();
    // This assumes that the largest dimension of the wheel is the diameter
    this->wheelRadius = bb.GetSize().GetMax() * 0.5;
}

void WBRobot::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    this->model = _parent;

    if (!_sdf->HasElement("left_joint"))
        gzerr << "WBRobot plugin missing <left_joint> element\n";

    if (!_sdf->HasElement("right_joint"))
        gzerr << "WBRobot plugin missing <right_joint> element\n";

    if (!_sdf->HasElement("laser"))
        gzerr << "WBRobot plugin missing <laser> element\n";


    this->leftJoint = _parent->GetJoint(
            _sdf->GetElement("left_joint")->GetValueString());
    this->rightJoint = _parent->GetJoint(
            _sdf->GetElement("right_joint")->GetValueString());

    if (_sdf->HasElement("torque"))
        this->torque = _sdf->GetElement("torque")->GetValueDouble();
    else
    {
        gzwarn << "No torque value set for the DiffDrive plugin.\n";
        this->torque = 5.0;
    }

    if (!this->leftJoint)
        gzerr << "Unable to find left joint["
            << _sdf->GetElement("left_joint")->GetValueString() << "]\n";
    if (!this->rightJoint)
        gzerr << "Unable to find right joint["
            << _sdf->GetElement("right_joint")->GetValueString() << "]\n";

    std::cout<<"load "<<model->GetName()<<""<<_sdf->GetElement("laser")->GetValueString()<<std::endl;
    laser =boost::shared_dynamic_cast<sensors::RaySensor>
        (sensors::get_sensor(this->model->GetName()+"::"+_sdf->GetElement("laser")->GetValueString()));
    if(laser)
        std::cout<<laser->GetName()<<":"<<laser->GetPose()<<std::endl;

    // this->updateTimestamp = common::Time::GetWallTime();


    this->updateConnection = event::Events::ConnectWorldUpdateStart(
            boost::bind(&WBRobot::OnUpdate, this));
}

void WBRobot::OnUpdate()
{
   // std::cout<<common::Time::GetWallTime()<<": "<<this->model->GetName()<<" "<<this->model->GetId()<<" on update"<<std::endl;
   // std::cout<<common::Time::GetWallTime()<<" : updateTime: "<<this->updateTimestamp<<" diff: "<<common::Time::GetWallTime() - this->updateTimestamp<<" comm: "<<common::Time(2,0)<<std::endl;
    
    if (common::Time::GetWallTime() - this->updateTimestamp > common::Time(1,10000000))
    {
        std::cout<<this->updateTimestamp<<": "<<std::endl;
        std::cout<<"---------------------------------------------------------------------------------------------"<<std::endl;
        std::vector<double> ranges;
        if(laser)
            laser->GetRanges(ranges);
        for(int i=0;i<ranges.size();i++)
            std::cout<<ranges[i]<<"\t";
        std::cout<<std::endl; 
        this->updateTimestamp = common::Time::GetWallTime();
    }

    SetSpeed(0.1,0.1);

    //update speed
    double leftVelDesired = (this->wheelSpeed[LEFT] / this->wheelRadius);
    double rightVelDesired = (this->wheelSpeed[RIGHT] / this->wheelRadius);

    this->leftJoint->SetVelocity(0, leftVelDesired);
    this->rightJoint->SetVelocity(0, rightVelDesired);

    this->leftJoint->SetMaxForce(0, this->torque);
    this->rightJoint->SetMaxForce(0, this->torque);
}


void WBRobot::SetSpeed(double lspeed, double rspeed)
{
    this->wheelSpeed[LEFT] = lspeed;
    this->wheelSpeed[RIGHT] = rspeed;
}

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(WBRobot)
