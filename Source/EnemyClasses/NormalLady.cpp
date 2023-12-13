#include "NormalLady.h"
#include "../Actors/Task_Player.h"
NormalLady::NormalLady()
	:Lady()
{
	AddComponent(fanEdge_=shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
	movement_->SetConsiderationCollition(true);
	gravity_->SetConsiderationCollition(true);
}

void NormalLady::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (!OutOfScreen())
		{
			afterState = AIState::Approach;
		}
		break;
	case AIState::Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = AIState::Attack;
		}
		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			afterState = AIState::Approach;
		}
		break;
	}
	UpDateState(afterState);
}

void NormalLady::Move()
{
	ML::Vec2 est;

	//d—Í‰Á‘¬
	if (!CheckFoot() || GetGravity()->GetVelocity().y)
	{

		gravity_->Accel();
	}
	else
	{
		SetMoveVecY(0.f);
	}

	switch (GetNowState())
	{
	case AIState::Idle:
		break;
	case AIState::Patrol:
		UpDatePatrol();
		break;
	case AIState::Approach:
		UpDateApproach();
		break;
	case AIState::Attack:
		
		if (!IsAttacking())
		{
			BeginAttack();
			moveCnt_->Start();
		}
		else
		{
			UpDateAttack();
		}
		break;
	case AIState::Damage:
		UpDateDamage();
		break;
	case AIState::Dead:
		UpDateDead();
		break;
	}

	est = GetMoveVec();
	CheckMove(est);
}

void NormalLady::UpDatePatrol()
{

}

void NormalLady::UpDateApproach()
{
	AIMove_->MoveTo(GetTarget());
}

void NormalLady::UpDateJump()
{

}

void NormalLady::UpDateFall()
{

}

void NormalLady::UpDateAttack()
{
	if (IsAttacking())
	{
		SetMoveVecX(0);
		fanEdge_->getHitBase().Offset(GetPos());
		if (fanEdge_->CheckHit(GetTarget()->GetBox()->getHitBase()))
		{
			static_cast<Character*>(GetTarget())->GetHP()->TakeDamage(2);
		}
		if (!moveCnt_->IsCounting())
		{
			EndAttack();
			return;
		}
	}
}

void NormalLady::UpDateGuard()
{

}

void NormalLady::UpDateDodge()
{

}

void NormalLady::UpDateDamage()
{

}

void NormalLady::UpDateDead()
{

}