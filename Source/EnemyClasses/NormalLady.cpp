#include "NormalLady.h"
#include "../Actors/Task_Player.h"
NormalLady::NormalLady()
	:Lady()
	, standCnt_(60)
	, attackCnt_(90)
	, adjustRange_(30.f)
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
		SetDistance(abs(ML::Vec2(GetPos() - GetTarget()->GetPos()).Length()));
		if (abs(GetFov()>GetDistance()))
		{
			afterState = AIState::Approach;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = static_cast<Player*>(GetTarget())->GetAttackBox();
			if (box_->CheckHit(plBox))
			{
				ge->debugFont->Draw(ML::Box2D{ 100,100,500,500 }, "true");
				EndAttack();
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = AIState::AttackStand;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = static_cast<Player*>(GetTarget())->GetAttackBox();
			if (box_->CheckHit(plBox))
			{
				ge->debugFont->Draw(ML::Box2D{ 100,100,500,500 }, "true");
				EndAttack();
				afterState = AIState::Damage;
			}
		}		break;
	case AIState::AttackStand:
		if (IsAttacking())
		{
			afterState = AIState::Attack;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = static_cast<Player*>(GetTarget())->GetAttackBox();
			if (box_->CheckHit(plBox))
			{
				ge->debugFont->Draw(ML::Box2D{ 100,100,500,500 }, "true");
				EndAttack();
				afterState = AIState::Damage;
			}
		}		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			afterState = AIState::Approach;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = static_cast<Player*>(GetTarget())->GetAttackBox();
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
		else if(!moveCnt_->IsCounting())
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

void NormalLady::Move()
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

void NormalLady::UpDateAttackStand()
{
	SetMoveVecX(0);
	if (!moveCnt_->IsCounting())
	{
		BeginAttack();
	}
}

void NormalLady::UpDateAttack()
{

	//攻撃中なら斬撃判定を出す
	if (IsAttacking())
	{
		SetMoveVecX(0);
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		//左右の斬撃判定の位置を調整
		if (angle_LR_ == Angle_LR::Left)
		{
			fanEdge_->getHitBase().Offset(GetTarget()->GetPos().x - adjustRange_, GetTarget()->GetPos().y);
		}
		else
		{
			fanEdge_->getHitBase().Offset(GetTarget()->GetPos().x + adjustRange_, GetTarget()->GetPos().y);
		}

		if (fanEdge_->CheckHit(plBox))
		{
			static_cast<Player*>(GetTarget())->TakeAttack(status_->attack.GetNow());
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
	if (!unHitTimer_->IsCounting())
	{
		status_->HP.TakeDamage(15);
		unHitTimer_->Start();
	}
	if (moveCnt_->IsCounting())
	{
		AIMove_->KnockBack();
	}
	
}

void NormalLady::UpDateDead()
{
	//仮ですぐ死ぬように
	this->Kill();
}

float NormalLady::GetAdjustRange() const
{
	return adjustRange_;
}