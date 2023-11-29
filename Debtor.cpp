#include "Debtor.h"

Debtor::Debtor()
	:Enemy()
	,preState_(AIState::Patrol)
	,nowState_(AIState::Patrol)
{
	movement_->SetConsiderationCollition(true);
	gravity_->SetConsiderationCollition(true);
}

void Debtor::Think()
{
	AIState afterState = nowState_;
	switch (afterState)
	{
	case AIState::Idle:
		if (!OutOfScreen())
			afterState = AIState::Patrol;
		break;
	//case AIState::Patrol:
	//	if (GetDistance() <= GetFov())
	//		afterState = AIState::Approach;
	//	break;
	//case AIState::Approach:
	//	if (GetDistance() <= GetRange())
	//	{
	//		afterState = AIState::Attack;
	//	}
	//	else if (GetDistance() > GetFov())
	//	{
	//		afterState = AIState::Patrol;
	//	}
	//	break;
	//case AIState::Attack:
	//	//射程外に出たら接近に切り替え
	//	if (GetDistance() > GetRange())
	//	{
	//		afterState = AIState::Approach;
	//	}

	}
	UpDateState(afterState);
}

void Debtor::Move()
{
	ML::Vec2 est;

	//重力加速
	if (!CheckFoot() || GetGravity()->GetVelocity().y)
	{

		gravity_->Accel();
	}
	else
	{
		SetMoveVecY(0.f);
		//gravity_->Stop();
	}

	switch (nowState_)
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
	AIMove_->Patroll(GetTarget());
}

void Debtor::UpDateApproach()
{

}

void Debtor::UpDateJump()
{

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

bool Debtor::UpDateState(AIState afterState)
{
	if (nowState_ == afterState)
		return false;
	else
	{
		preState_ = nowState_;
		nowState_ = afterState;
		return true;
	}
}

bool Debtor::HitPlayer()
{
	if (CheckHit(GetTarget()->GetBox()->getHitBase()))
	{
		//プレイヤーに当たった時の処理
		static_cast<Character*>(GetTarget())->GetHP()->TakeDamage(2);
		this->Kill();
		return true;
	}
	return false;
}