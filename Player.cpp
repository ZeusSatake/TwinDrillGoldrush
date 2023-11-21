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

void Player::Move()
{
	switch (pState)
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