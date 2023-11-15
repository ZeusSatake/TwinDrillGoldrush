#include "AIMoveComponent.h"

AIMoveComponent::AIMoveComponent(Actor* owner)
	:Component(owner)
{
	owner_ = owner;
}

void AIMoveComponent::Update()
{
	switch (state_)
	{
	case AIState::Idle:
		break;
	case AIState::Patrol:
		break;
	case AIState::Attack:
		break;
	case AIState::Jump:
		break;
	case AIState::Dead:
		break;
	}
}

void AIMoveComponent::UpdatePatrol()
{

}

void AIMoveComponent::UpdateJump()
{

}

void AIMoveComponent::UpdateDead()
{

}

void AIMoveComponent::UpdateAttack()
{

}

void AIMoveComponent::MoveTo(class Actor* target)
{
	
}

void AIMoveComponent::Patroll(class Actor* target)
{

}

void AIMoveComponent::Jump()
{

}