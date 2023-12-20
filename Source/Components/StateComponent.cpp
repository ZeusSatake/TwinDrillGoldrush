#include "StateComponent.h"
StateComponent::StateComponent(Actor* owner)
	:
	Component(owner),
	nowState(State::Idle),
	preState(State::Idle),
	moveCnt_(0)
{
	owner_ = owner;
}

void StateComponent::UpdateNowState(State state)
{
	if (this->preState == state)return;
	this->moveCnt_ = 0;
	this->preState = this->nowState;
	this->nowState = state;
}

void StateComponent::InitializeState()
{
	this->UpdateNowState(State::Idle);
}

void StateComponent::InitializeState(State state)
{
	this->nowState = state;
}

StateComponent::State StateComponent::GetNowState()
{
	return this->nowState;
}