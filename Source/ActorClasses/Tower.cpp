#include "Tower.h"
#include "../Actors/Task_Player.h"

Tower::Tower()
	:Actor()
	, movementAmount_(3.5f)
	, noticeCnt_(300)
	, decisionCnt_(120)
	, stopCnt_(90)
	, nowState_(TowerState::Notice)
	, startPosY_(ge->screen2DHeight - 20)
	, damage(500)
{
	AddComponent(moveCount_ = shared_ptr<TimerComponent>(new TimerComponent(this)));

	box_->setHitBase(ML::Box2D{ -32, 0, 64, 512});//先端に合わせる
}

void Tower::Think()
{
	TowerState afterState = nowState_;
	switch (nowState_)
	{
		case TowerState::Notice:
			if (!moveCount_->IsCounting())
			{
				afterState = TowerState::Decision;
			}
		break;
		case TowerState::Decision:
			if (!moveCount_->IsCounting())
			{
				afterState = TowerState::Rise;
			}
		break;
		case TowerState::Rise:
			if (GetPos().x<=0)
			{
				afterState = TowerState::Stop;
			}
		break;
		case TowerState::Stop:
			if (!moveCount_->IsCounting())
			{
				afterState = TowerState::Down;
			}
		break;
		case TowerState::Down:
			if (GetPos().y<=startPosY_)
			{
				afterState = TowerState::Notice;
			}
		break;
	}
	//状態の更新と各状態ごとの行動カウンタを設定
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case TowerState::Notice:
			moveCount_->SetCountFrame(noticeCnt_);
			break;
		case TowerState::Decision:
			moveCount_->SetCountFrame(decisionCnt_);
			break;
		case TowerState::Stop:
			moveCount_->SetCountFrame(stopCnt_);
			break;
		default:
			moveCount_->SetCountFrame(0);
			break;
		}
		moveCount_->Start();
	}
}

void Tower::Move()
{
	switch (nowState_)
	{
		case TowerState::Notice:
			UpDateNotice();
		break;
		case TowerState::Decision:
			UpDateDecision();
			break;
		case TowerState::Rise:
			UpDateRise();
			break;
		case TowerState::Stop:
			UpDateStop();
			break;
		case TowerState::Down:
			UpDateDown();
			break;
			
	}
}

void Tower::HitPlayer()
{
	if (ge->playerPtr->pState != StateComponent::State::Damage)
	{
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());

		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->GetStatus()->HP.TakeDamage(damage);
		}
	}
}

void Tower::UpDateNotice()
{
	SetPosX(ge->playerPtr->GetPos().x);
}

void Tower::UpDateDecision()
{
}

void Tower::UpDateRise()
{
	SetMoveVecY(movementAmount_);
}

void Tower::UpDateStop()
{
}

void Tower::UpDateDown()
{
	SetMoveVecY(-movementAmount_);
}

bool Tower::UpDateState(TowerState afterState)
{
	if (nowState_ == afterState) return false;
	else
	{
		nowState_ = afterState;
		return true;
	}
}