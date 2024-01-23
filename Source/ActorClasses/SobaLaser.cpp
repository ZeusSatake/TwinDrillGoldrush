#include "SobaLaser.h"
#include "../Actors/Task_Player.h"

SobaLaser::SobaLaser()
	:Actor()
	,countFlag_(false)
	,initFlag_(false)
	,damage_(10)
{
	AddComponent(suviveTimer_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	suviveTimer_->SetCountFrame(180);
	suviveTimer_->Start();
	box_->setHitBase(ML::Box2D{ 0, 0, 0, 0 });
}

void SobaLaser::Iniitialize()
{
	if (direction_ == Down)
	{
		box_->setHitBase(ML::Box2D{ -4, 0, 8, 32 });
	}
	else if (direction_ == Left)
	{
		box_->setHitBase(ML::Box2D{ -32, -4, 32, 8 });
	}
	else if (direction_ == Right)
	{
		box_->setHitBase(ML::Box2D{ 0, -4, 32, 8 });
	}
}

void SobaLaser::Move()
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

void SobaLaser::MoveByDirection()
{
	switch (direction_)
	{
	case Direction::Down:
		SetMoveVecX(0);
		SetMoveVecY(movementAmount_);
		break;
	case Direction::Left:
		SetMoveVecX(-movementAmount_);
		SetMoveVecY(0);
		break;
	case Direction::Right:
		SetMoveVecX(movementAmount_);
		SetMoveVecY(0);
		break;
	}
	SetPos(GetPos() + GetMoveVec());
}