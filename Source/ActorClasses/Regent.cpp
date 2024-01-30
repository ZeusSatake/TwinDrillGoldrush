#include "Regent.h"
#include "../Actors/Task_Player.h"

Regent::Regent()
	: Actor()
	, countFlag_(false)
	, initFlag_(false)
	, damage_(0)
{
	AddComponent(suviveTimer_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	suviveTimer_->SetCountFrame(180);
	suviveTimer_->Start();
	box_->setHitBase(ML::Box2D{ 0, 0, 0, 0 });
}

void Regent::Move()
{
	if (!initFlag_)
	{
		Iniitialize();
	}
	suviveTimer_->Update();

	//ˆÚ“®ˆ—
	MoveByDirection();


	if (!countFlag_)
	{
		suviveTimer_->Start();
		countFlag_ = true;
	}
	if (!suviveTimer_->IsCounting())
	{
		Kill();
	}

	ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
	plBox.Offset(ge->playerPtr->GetPos());
	if (box_->CheckHit(plBox))
	{
		ge->playerPtr->TakeAttack(damage_);
	}
}

void Regent::Iniitialize()
{
	if (angle_LR_ == Angle_LR::Left)
	{
		box_->setHitBase(ML::Box2D{ -16, -4, 16, 8 });
	}
	else
	{
		box_->setHitBase(ML::Box2D{ 0, -4, 16, 8 });
	}
}

void Regent::MoveByDirection()
{
	if (angle_LR_ == Angle_LR::Left)
	{
		SetMoveVecX(-movementAmount_);
		SetMoveVecY(0);
	}
	else
	{
		SetMoveVecX(movementAmount_);
		SetMoveVecY(0);
	}
	SetPos(GetPos() + GetMoveVec());
}
