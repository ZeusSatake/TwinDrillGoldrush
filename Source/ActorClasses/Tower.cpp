#include "Tower.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Chandelier.h"

Tower::Tower()
	:Actor()
	, movementAmount_(3.5f)
	, noticeCnt_(300)
	, decisionCnt_(90)
	, stopCnt_(90)
	, nowState_(TowerState::Idle)
	, startPosY_(ge->screen2DHeight - 10)
	, damage_(250)
	, isfellChandelier_(false)
{
	AddComponent(moveCount_ = shared_ptr<TimerComponent>(new TimerComponent(this)));

	SetPosY(startPosY_);
	box_->setHitBase(ML::Box2D{ -32, 0, 64, 512});//先端に合わせる
}

void Tower::Think()
{
	TowerState afterState = nowState_;
	switch (afterState)
	{
	case TowerState::Idle:
		if (ge->playerPtr)
		{
			afterState = TowerState::Notice;
		}
		break;
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
		if (GetPos().y <= 0)
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
		if (GetPos().y > startPosY_)
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
		}
		moveCount_->Start();
	}
}

void Tower::Move()
{
	moveCount_->Update();
	switch (nowState_)
	{
	case TowerState::Idle:
		break;
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
	SetPosX(GetPos().x + GetMoveVec().x);
	SetPosY(GetPos().y + GetMoveVec().y);

	HitPlayer();
}

void Tower::HitPlayer()
{
	ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
	plBox.Offset(ge->playerPtr->GetPos());

	if (box_->CheckHit(plBox))
	{
		ge->playerPtr->TakeAttack(damage_);
	}
}

void Tower::UpDateNotice()
{
	SetMoveVecY(0);
	SetPosX(ge->playerPtr->GetPos().x);
}

void Tower::UpDateDecision()
{
}

void Tower::UpDateRise()
{
	SetMoveVecY(-movementAmount_);
}

void Tower::UpDateStop()
{
	if (!isfellChandelier_)
	{
		auto ch0 = Chandelier0::Object::Create(true);
		ch0->SetPos(ML::Vec2{GetPos().x + 128, GetPos().y});
		auto ch1 = Chandelier0::Object::Create(true);
		ch1->SetPos(ML::Vec2{GetPos().x + 256, GetPos().y});
		auto ch2 = Chandelier0::Object::Create(true);
		ch2->SetPos(ML::Vec2{GetPos().x - 128, GetPos().y});
		auto ch3 = Chandelier0::Object::Create(true);
		ch3->SetPos(ML::Vec2{GetPos().x - 256, GetPos().y});
		isfellChandelier_ = true;
	}
}

void Tower::UpDateDown()
{
	SetMoveVecX(0);
	SetMoveVecY(movementAmount_);
	isfellChandelier_ = false;
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