#include "DebtorAIComponent.h"

DebtorAIComponent::DebtorAIComponent(class Debtor* owner)
	:EnemyAIComponent(owner)
{
	fov_ = 150.f;
	
}
void DebtorAIComponent::Think()
{
	switch(preState_)
	{
	case AIState::Idle:
		if (owner_->OutOfScreen())
			nowState_ = AIState::Patrol;
		break;
	case AIState::Patrol:
		if (distance_ <= fov_)
			nowState_ = AIState::Approach;
		break;
	case AIState::Approach:
		if (distance_ <= static_cast<Enemy*>(owner_)->range_)
		{

		}
		break;
	}
}

void DebtorAIComponent::Move()
{
	//èdóÕâ¡ë¨
	if (!static_cast<Character*>(owner_)->CheckFoot() || owner_->GetMoveVec().y < 0)
	{
		owner_->SetMoveVec(ML::Vec2(0, min(owner_->GetMoveVec().y + owner_->GetGravity(), owner_->GetMaxFallSpeed())));
	}
	else
	{
		owner_->SetMoveVec(ML::Vec2{ 0, 0 });
	}

	switch (nowState_)
	{
	case AIState::Idle:
		break;
	case AIState::Patrol:
		UpdatePatrol();
		break;
	case AIState::Approach:
		UpdateApproach();
		break;
	case AIState::Attack:
		UpdateAttack();
		break;
	case AIState::Damage:
		UpdateDamage();
		break;
	case AIState::Dead:
		UpdateDead();
		break;
	}
}

void DebtorAIComponent::Update()
{
	Think();
	Move();
}

void DebtorAIComponent::UpdatePatrol()
{
	//
	//ML::Vec2 toTarget = target_->pos_ - owner_->pos_;
	//distance_ = toTarget.Length();


}
void DebtorAIComponent::UpdateApproach()
{

}
void DebtorAIComponent::UpdateAttack()
{

}
void DebtorAIComponent::UpdateJump()
{

}
void DebtorAIComponent::UpdateDead()
{

}
void DebtorAIComponent::UpdateDodge()
{

}
void DebtorAIComponent::UpdateDamage()
{

}
void DebtorAIComponent::UpdateGuard()
{

}


