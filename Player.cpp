#include "Player.h"
#include "Source/Scene/Task_Map.h"

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
		if (true) { pState = StateComponent::State::Walk; }
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
	auto inp = this->controller_->gamePad_->GetState();
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
		/*if (inp.LStick.BU.on) { preVec.y -= 3; }
		if (inp.LStick.BD.on) { preVec.y += 3; }
		if (inp.LStick.BR.on) { preVec.x += 3; }
		if (inp.LStick.BL.on) { preVec.x -= 3; }*/
		preVec+=controller_->GetLStickVec();
		
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
	this->PlayerMove(preVec);
}

ML::Vec2 Player::GetPos()
{
	return this->pos_;
}

ML::Vec2 Player::Gravity()
{
	
}