#include "ControllerInputComponent.h"
#include "../../XI2018_Ver1_0.h"

ControllerInputComponent::ControllerInputComponent(class Actor* owner)
	:
	Component((Actor*)owner),
	gamePad_(ge->in1)
{
	owner = owner_;
}

void ControllerInputComponent::SetGamePadId(XI::GamePad::SP contoroller)
{
	this->gamePad_ = contoroller;
}


float ControllerInputComponent::GetAngleLStick()
{
	auto inp = this->gamePad_->GetState();
	return inp.LStick.angleDYP;
}

float ControllerInputComponent::GetAngleRStick()
{
	auto inp = this->gamePad_->GetState();
	return inp.RStick.angleDYP;
}


ML::Vec2 ControllerInputComponent::GetLStickVec()
{
	auto inp = this->gamePad_->GetState();
	float angle;

	angle = inp.LStick.angleDYP;
	
	ML::Vec2 moveVec{ cos(angle),sin(0.0f) };
	if (inp.LStick.volume == 0)
	{
		moveVec = ML::Vec2{ 0,0 };
	}
	return moveVec;
}

ML::Vec2 ControllerInputComponent::GetRStickVec()
{
	auto inp = this->gamePad_->GetState();
	float angle;
	if (inp.RStick.angleDYP >= 0.0f)
	{
		angle = inp.RStick.angleDYP;
	}
	ML::Vec2 moveVec{ cos(angle),sin(angle) };
	return moveVec;
}

