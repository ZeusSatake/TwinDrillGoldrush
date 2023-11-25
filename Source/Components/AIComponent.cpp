#include "AIComponent.h"
AIComponent::AIComponent(Character* owner)
	:Component((Actor*)owner)
	,preState_(AIState::Idle)
	,nowState_(AIState::Idle)
{
	move_ = shared_ptr<AIMoveComponent>(new AIMoveComponent(owner));
}

void AIComponent::Update()
{
	switch(nowState_)
	{
	case AIComponent::AIState::Idle:
		break;
	case AIComponent::AIState::Patrol:
		break;
	case AIComponent::AIState::Approach:
		break;
	case AIComponent::AIState::Jump:
		break;
	case AIComponent::AIState::Attack:
		break;
	case AIComponent::AIState::Guard:
		break;
	case AIComponent::AIState::Dodge:
		break;
	case AIComponent::AIState::Dead:
		break;
	default:
		break;
	}
}

void AIComponent::UpdatePatrol()
{

}

void AIComponent::UpdateApproach()
{

}

void AIComponent::UpdateJump()
{

}

void AIComponent::UpdateAttack()
{

}

void AIComponent::UpdateGuard()
{

}

void AIComponent::UpdateDodge()
{

}

void AIComponent::UpdateDamage()
{

}

void AIComponent::UpdateDead()
{

}

void AIComponent::Think()
{

}

void AIComponent::Move()
{

}

Actor* AIComponent::GetTarget()
{
	return target_;
}

void AIComponent::SetTarget(Actor* target)
{
	target_ = target;
}

