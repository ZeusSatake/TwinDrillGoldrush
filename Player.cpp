#include "Player.h"

Player::Player()
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}

void Player::PlayerMove(ML::Vec2 vec)
{
	this->pos_ += vec;
}

void Player::Think()
{
	auto inp = controller_->gamePad_->GetState();
	switch (pState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		if (inp.B1.down) { pState = StateComponent::State::Walk; }
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
	state_->UpdateNowState(pState);
}

void Player::Move()
{
	ML::Vec2 preVec{0,0};
	if (state_->GetNowState() == StateComponent::State::Fall)
	{
		
	}
	switch (state_->GetNowState())
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
	//ここに最終的にマップとの移動可否チェックを入れる
    //this->CheckHitMap(this->preVec);
}