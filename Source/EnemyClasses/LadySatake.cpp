#include "LadySatake.h"
#include "../Actors/Task_Player.h"
LadySatake::LadySatake()
	:BossLady()	
	, standCnt_(60)
    , attackCnt_(90)
{
}

void LadySatake::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (WithinSight(GetTarget()))
		{
			afterState = AIState::Approach;
		}
		break;
	case AIState::Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = AIState::AttackStand;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::AttackStand:
		if (IsAttacking())
		{
			afterState = AIState::Attack;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				EndAttack();
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			afterState = AIState::Approach;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				EndAttack();
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::Damage:
		if (status_->HP.GetNowHP() <= 0)
		{
			afterState = AIState::Dead;
		}
		else if (!moveCnt_->IsCounting())
		{
			afterState = AIState::Approach;
		}
		break;
	case AIState::Dead:
		break;
	}
	//状態の更新と各状態ごとの行動カウンタを設定
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::AttackStand:
			moveCnt_->SetCountFrame(standCnt_);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(attackCnt_);
			break;
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

void LadySatake::Move()
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

void LadySatake::UpDateApproach()
{

}

void LadySatake::UpDateJump()
{

}

void LadySatake::UpDateFall()
{

}

void LadySatake::UpDateAttackStand()
{

}

void LadySatake::UpDateAttack()
{
	//攻撃中なら斬撃判定を出す
	if (IsAttacking())
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		switch (GetAttackPattern())
		{
		case 0:
			SetMoveVecX(0);
			if (angle_LR_ == Angle_LR::Left)
			{
				fanEdge_->getHitBase().Offset(GetTarget()->GetPos().x, GetTarget()->GetPos().y);
			}
			else
			{
				fanEdge_->getHitBase().Offset(GetTarget()->GetPos().x, GetTarget()->GetPos().y);
			}

			if (fanEdge_->CheckHit(plBox))
			{
				static_cast<Player*>(GetTarget())->status_->HP.TakeDamage(status_->attack.GetNow());
			}
			if (!moveCnt_->IsCounting())
			{
				EndAttack();
				return;
			}
			break;
		case 1:
			SetMoveVecX(10);
			break;
		}
	}
}

void LadySatake::UpDateDamage()
{

}

void LadySatake::UpDateDead()
{
	this->Kill();
}