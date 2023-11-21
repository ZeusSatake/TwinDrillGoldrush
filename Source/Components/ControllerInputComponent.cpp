#include "ControllerInputComponent.h"
#include "../../XI2018_Ver1_0.h"

ControllerInputComponent::ControllerInputComponent(Actor* owner)
	:
	Component(owner),
	controller_(ge->in1)
{
	owner_ = owner;
}

void ControllerInputComponent::SetGamePadId(XI::GamePad::SP contoroller)
{
	this->controller_ = contoroller;
}


float ControllerInputComponent::GetAngleLStick()
{
	auto inp = this->controller_->GetState();
	return inp.LStick.angleDYP;
}

float ControllerInputComponent::GetAngleRStick()
{
	auto inp = this->controller_->GetState();
	return inp.RStick.angleDYP;
}


ML::Vec2 ControllerInputComponent::GetLStickVec()
{
	auto inp = this->controller_->GetState();
	float angle;
	if (inp.LStick.angleDYP >= 0.0f)
	{
		angle = inp.LStick.angleDYP;
	}
	ML::Vec2 moveVec{ cos(angle),sin(angle) };
	return moveVec;
}

ML::Vec2 ControllerInputComponent::GetRStickVec()
{
	auto inp = this->controller_->GetState();
	float angle;
	if (inp.RStick.angleDYP >= 0.0f)
	{
		angle = inp.RStick.angleDYP;
	}
	ML::Vec2 moveVec{ cos(angle),sin(angle) };
	return moveVec;
}

