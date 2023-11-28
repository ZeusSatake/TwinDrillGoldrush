#include "Drill.h"
#include "MyPG.h"
Drill::Drill()
	:
	attackPoint(0),
	nowAngle(0.0f),
	durability(0)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}

void Drill::InitPos(ML::Vec2 pos)
{
	this->pos_ = pos;
}

void Drill::SetAngle(float angle)
{
	this->angle_ = angle;
}

int Drill::GetAttackPoint()
{
	return this->attackPoint;
}

float Drill::GetNowAngle()
{
	return this->nowAngle;
}

ML::Vec2 Drill::GetDrillPoint()
{
	return this->pos_;
}

float Drill::UpdateDrillAngle()
{
	auto inp = controller_->gamePad_->GetState();
	
	return inp.RStick.angleDYP;
	/*if (inp.RStick.BU.on) { return ML::ToRadian(90); }
	else return 0;*/
}

void Drill::Think()
{
	auto inp = controller_->gamePad_->GetState();

	switch (dState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		break;
	case StateComponent::State::Walk:
		break;
	case StateComponent::State::Attack:
		break;
	case StateComponent::State::SpinAttack:
		break;
	case StateComponent::State::Damage:
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		break;
	case StateComponent::State::Fall:
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
		break;
	case StateComponent::State::Appeal:
		break;
	}
	state_->UpdateNowState(dState);
}

void Drill::Move()
{
	auto inp = controller_->gamePad_->GetState();

	switch (dState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		break;
	case StateComponent::State::Walk:
		break;
	case StateComponent::State::Attack:
		break;
	case StateComponent::State::SpinAttack:
		break;
	case StateComponent::State::Damage:
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		break;
	case StateComponent::State::Fall:
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
		break;
	case StateComponent::State::Appeal:
		break;

	}
}