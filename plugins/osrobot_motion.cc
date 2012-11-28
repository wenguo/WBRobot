#include "osrobot_motion.hh"
using namespace gazebo;

OSRobot::OSRobot()
{
  this->joints.resize(4);

  this->aeroLoad = 0;
  this->swayForce = 0;
  this->frontPower=20;
  this->rearPower=20;
  this->maxSpeed=1.0;

  this->following_camera = false;
}

OSRobot::~OSRobot()
{
}

void OSRobot::Init()
{
  this->chassis = this->joints[0]->GetParent();

  // This assumes that the largest dimension of the wheel is the diameter
  physics::EntityPtr parent = boost::shared_dynamic_cast<physics::Entity>(
      this->joints[0]->GetChild());
  math::Box bb = parent->GetBoundingBox();
  this->wheelRadius = bb.GetSize().GetMax() * 0.5;
}

void OSRobot::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    this->model = _parent;

 this->joints[0] = this->model->GetJoint(_sdf->GetValueString("front_left"));
  this->joints[1] = this->model->GetJoint(_sdf->GetValueString("front_right"));
  this->joints[2] = this->model->GetJoint(_sdf->GetValueString("back_left"));
  this->joints[3] = this->model->GetJoint(_sdf->GetValueString("back_right"));
    
  this->frontPower = _sdf->GetValueDouble("front_power");
  this->rearPower = _sdf->GetValueDouble("rear_power");

  this->joints[0]->SetAttribute("suspension_erp", 0, 0.15);
  this->joints[0]->SetAttribute("suspension_cfm", 0, 0.04);

  this->joints[1]->SetAttribute("suspension_erp", 0, 0.15);
  this->joints[1]->SetAttribute("suspension_cfm", 0, 0.04);

  this->joints[2]->SetAttribute("suspension_erp", 0, 0.15);
  this->joints[2]->SetAttribute("suspension_cfm", 0, 0.04);

  this->joints[3]->SetAttribute("suspension_erp", 0, 0.15);
  this->joints[3]->SetAttribute("suspension_cfm", 0, 0.04);

  if (!this->joints[0])
  {
    gzerr << "Unable to find front_left joint["
          << _sdf->GetElement("front_left") << "]\n";
    return;
  }

  if (!this->joints[1])
  {
    gzerr << "Unable to find front_right joint["
          << _sdf->GetElement("front_right") << "]\n";
    return;
  }

  if (!this->joints[2])
  {
    gzerr << "Unable to find back_left joint["
          << _sdf->GetElement("back_left") << "]\n";
    return;
  }

  if (!this->joints[3])
  {
    gzerr << "Unable to find back_right joint["
          << _sdf->GetElement("back_right") << "]\n";
    return;
  }
  this->connections.push_back(event::Events::ConnectWorldUpdateStart(
          boost::bind(&OSRobot::OnUpdate, this)));

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init(this->model->GetWorld()->GetName());

  this->velSub = this->node->Subscribe(std::string("~/") +
      this->model->GetName() + "/vel_cmd", &OSRobot::OnVelMsg, this);

  this->reqSub = this->node->Subscribe(std::string("~/") +
          this->model->GetName() + "/req", &OSRobot::OnReqMsg, this);


}

void OSRobot::OnUpdate()
{
  
  // Compute the angle of the front wheels.
  double wheelAngle = GZ_DTOR(this->turnAngle);;

  // Compute the rotational velocity of the wheels
  double jointVel = this->forwardSpeed /this->wheelRadius;

  // Set velocity and max force for each wheel
  this->joints[0]->SetVelocity(1, -jointVel);
  this->joints[0]->SetMaxForce(1, (this->forwardSpeed / this->maxSpeed) * this->frontPower);

  this->joints[1]->SetVelocity(1, -jointVel);
  this->joints[1]->SetMaxForce(1, (this->forwardSpeed / this->maxSpeed) * this->frontPower);

  this->joints[2]->SetVelocity(1, -jointVel);
  this->joints[2]->SetMaxForce(1, (this->forwardSpeed / this->maxSpeed) * this->rearPower);

  this->joints[3]->SetVelocity(1, -jointVel);
  this->joints[3]->SetMaxForce(1, (this->forwardSpeed / this->maxSpeed) * this->rearPower);

  // Set the front-left wheel angle
  this->joints[0]->SetLowStop(0, wheelAngle);
  this->joints[0]->SetHighStop(0, wheelAngle);
  this->joints[0]->SetLowStop(0, wheelAngle);
  this->joints[0]->SetHighStop(0, wheelAngle);

  // Set the front-right wheel angle
  this->joints[1]->SetHighStop(0, wheelAngle);
  this->joints[1]->SetLowStop(0, wheelAngle);
  this->joints[1]->SetHighStop(0, wheelAngle);
  this->joints[1]->SetLowStop(0, wheelAngle);

  // Get the current velocity of the car
  this->velocity = this->chassis->GetWorldLinearVel();

  /*
  //  aerodynamics
  this->chassis->AddForce(
  math::Vector3(0, 0, this->aeroLoad * this->velocity.GetSquaredLength()));

  // Sway bars
  math::Vector3 bodyPoint;
  math::Vector3 hingePoint;
  math::Vector3 axis;

  double displacement;

  for (int ix = 0; ix < 4; ++ix)
  {
  hingePoint = this->joints[ix]->GetAnchor(0);
  bodyPoint = this->joints[ix]->GetAnchor(1);

  axis = this->joints[ix]->GetGlobalAxis(0).Round();
  displacement = (bodyPoint - hingePoint).Dot(axis);

  float amt = displacement * this->swayForce;
  if (displacement > 0)
  {
  if (amt > 15)
  amt = 15;

  math::Pose p = this->joints[ix]->GetChild()->GetWorldPose();
  this->joints[ix]->GetChild()->AddForce(axis * -amt);
  this->chassis->AddForceAtWorldPosition(axis * amt, p.pos);

  p = this->joints[ix^1]->GetChild()->GetWorldPose();
  this->joints[ix^1]->GetChild()->AddForce(axis * amt);
  this->chassis->AddForceAtWorldPosition(axis * -amt, p.pos);
  }
  }*/

  if(this->following_camera)
  {
      math::Pose p = this->chassis->GetWorldPose();

  }
}

void OSRobot::OnVelMsg(ConstPosePtr &_msg)
{
    this->forwardSpeed = _msg->position().x();
    this->turnAngle = _msg->position().y();
    std::cout<<"received vel message: ("<<this->forwardSpeed<<" , "<<this->turnAngle<<")"<<std::endl;

}

void OSRobot::OnReqMsg(ConstReqPtr &_msg)
{
    printf("Set following camera\n");

    this->following_camera = _msg->following_cam();
}

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(OSRobot)
