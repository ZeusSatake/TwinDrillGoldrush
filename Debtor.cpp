#include "Debtor.h"
#include "Source/Actors/Task_Player.h"

Debtor::Debtor()
	:Enemy()
{
	movement_->SetConsiderationCollition(true);
	gravity_->SetConsiderationCollition(true);
}

void Debtor::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case Idle:
		if (!OutOfScreen())
			afterState = AIState::Patrol;
		break;
	case Patrol:
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				afterState = AIState::Damage;
			}
		}
	case Damage:
		if (status_->HP.GetNowHP() <= 0)
		{
			afterState = AIState::Dead;
		}
		else if (!moveCnt_->IsCounting())
		{
			afterState = AIState::Patrol;
		}
		break;
	}
	UpDateState(afterState);
}

void Debtor::Move()
{
	ML::Vec2 est;

	HitPlayer();

	//重力加速
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

void Debtor::UpDatePatrol()
{
	AIMove_->Patroll();
}

void Debtor::UpDateApproach()
{

}

void Debtor::UpDateJump()
{
	AIMove_->Jump();
}

void Debtor::UpDateFall()
{

}

void Debtor::UpDateAttack()
{

}

void Debtor::UpDateGuard()
{

}

void Debtor::UpDateDodge()
{

}

void Debtor::UpDateDamage()
{
	
}

void Debtor::UpDateDead()
{

}

bool Debtor::HitPlayer()
{
	ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
	plBox.Offset(GetTarget()->GetPos());
	if (CheckHit(plBox))
	{
		//プレイヤーに当たった時の処理
		static_cast<Character*>(GetTarget())->GetHP()->TakeDamage(10);
		return true;
	}
	return false;
}