#include "DebtorAIComponent.h"
#include "../../Debtor.h"

DebtorAIComponent::DebtorAIComponent(class Debtor* owner)
	:EnemyAIComponent((Enemy*)owner)
{
	fov_ = 150.f;
	
}
void DebtorAIComponent::Think()
{

	AIState afterState = nowState_;
	switch(afterState)
	{
	case AIState::Idle:
		if (owner_->OutOfScreen())
			afterState = AIState::Patrol;
		break;
	case AIState::Patrol:
		if (distance_ <= fov_)
			afterState = AIState::Approach;
		break;
	case AIState::Approach:
		if (distance_ <= static_cast<Enemy*>(owner_)->range_)
		{
			afterState = AIState::Attack;
		}
		else if (distance_ > fov_)
		{
			afterState = AIState::Patrol;
		}
		break;
	case AIState::Attack:
		//�����W����O�ꂽ��ڋ�
		if (distance_ > static_cast<Enemy*>(owner_)->range_)
		{
			afterState = AIState::Approach;
		}

	}
	UpdateState(afterState);
}

void DebtorAIComponent::Move()
{
	//�d�͉���
	/*if (!static_cast<Character*>(owner_)->CheckFoot() || static_cast<Character*>(owner_)->GetMovement()->GetMoveVec().y)
	{
		static_cast<Character*>(owner_)->GetMovement()->SetMoveVec(ML::Vec2(owner_->GetMoveVec().x, min(owner_->GetMoveVec().y + owner_->GetGravity(), owner_->GetMaxFallSpeed())));
	}
	else
	{
		owner_->SetMoveVec(ML::Vec2{ owner_->GetMoveVec().x, 0 });
	}*/

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
	move_->Patroll(target_);
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

bool DebtorAIComponent::UpdateState(AIState afterState)
{
	if(nowState_== afterState)
		return false;
	else
	{
		preState_ = nowState_;
		nowState_ = afterState;
		return true;
	}
}

