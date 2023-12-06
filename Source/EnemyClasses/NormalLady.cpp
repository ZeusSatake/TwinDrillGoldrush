#include "NormalLady.h"

NormalLady::NormalLady()
	:Lady()
{
	movement_->SetConsiderationCollition(true);
	gravity_->SetConsiderationCollition(true);
}

void NormalLady::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case Idle:
		if (!OutOfScreen())
		{
			afterState = Approach;
		}
		break;
	case Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = Attack;
		}
		break;
	case Attack:

		break;
	}
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
		BeginAttack();
		moveCnt_->Start();
		UpDateAttack();
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