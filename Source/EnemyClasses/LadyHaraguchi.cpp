#include "LadyHaraguchi.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Tower.h"

LadyHaraguchi::LadyHaraguchi()
	:BossLady()
{
	
}

void LadyHaraguchi::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (WithinSight(GetTarget()))
		{
			afterState = AttackStand;
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
	case AIState::Attack:
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
	case AIState::Damage:
		if (status_->HP.GetNowHP() <= 0)
		{
			afterState = AIState::Dead;
		}
		else if (!moveCnt_->IsCounting())
		{
			afterState = AIState::AttackStand;
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
			moveCnt_->SetCountFrame(0);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(0);
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

void LadyHaraguchi::Move()
{
	if (!isCreatedTower_)
	{
		auto tw = Tower::Object::Create(true);
		tw->SetPosX(ge->playerPtr->GetPos().x);
	}
}
