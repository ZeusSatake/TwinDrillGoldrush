#include "Debtor.h"
#include "../Actors/Task_Player.h"

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
		break;
	case Damage:
		break;
	}
	if (status_->HP.GetNowHP() <= 0)
	{
		afterState = AIState::Dead;
	}

	//状態の更新と各状態ごとの行動カウンタを設定
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::Damage:
			moveCnt_->SetCountFrame(30);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void Debtor::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();

	ML::Vec2 est;

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

	HitPlayer();
	UpDateHP();
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
	if (moveCnt_->IsCounting())
	{
		AIMove_->KnockBack();
	}
}

void Debtor::UpDateDead()
{
	this->Kill();
}

void Debtor::HitPlayer()
{
	//ダメージ処理
	if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
	{
		ML::Box2D plBox = ge->playerPtr->drill_->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->drill_->GetAttackPos());
		if (box_->CheckHit(plBox))
		{
			GetStatus()->HP.TakeDamage(ge->playerPtr->GetStatus()->attack.GetNow());
		}
	}

	if (GetNowState() != AIState::Dead)
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->TakeAttack(GetStatus()->attack.GetNow());
		}
	}
}
