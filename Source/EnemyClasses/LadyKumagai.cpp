#include"LadyKumagai.h"
#include"../Actors/Task_Player.h"

LadyKumagai::LadyKumagai()
	:BossLady()
	, standCnt_(60)
	, attackCnt_(90)
{
}

void LadyKumagai::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		break;
	case AIState::Approach:
		break;
	case AIState::AttackStand:
		break;
	case AIState::Attack:
		break;
	case AIState::Damage:
		break;
	case AIState::Dead:
		break;
	}
}

void LadyKumagai::Move()
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
	case AIState::AttackStand:
		UpDateAttackStand();
		break;
	case AIState::Attack:
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

void LadyKumagai::UpDateApproach()
{
	AIMove_->MoveTo(GetTarget());
}

void LadyKumagai::UpDateJump()
{
}

void LadyKumagai::UpDateFall()
{
}

void LadyKumagai::UpDateAttackStand()
{
}

void LadyKumagai::UpDateAttack()
{
}

void LadyKumagai::UpDateDamage()
{
}

void LadyKumagai::UpDateDead()
{
}

