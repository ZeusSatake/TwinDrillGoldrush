#include "SobaPrison.h"
#include "../Actors/Task_Player.h"

SobaPrison::SobaPrison()
	:Actor()
	, state_(State::Notice)
	, damage_(50)
	, countFlag_(false)
{
	AddComponent(expandTimer_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	expandTimer_->SetCountFrame(120);
	box_->setHitBase(ML::Box2D{ -4, 0, 8, 256 });
}

void SobaPrison::Move()
{
	expandTimer_->Update();
	if (!countFlag_)
	{
		expandTimer_->Start();
		countFlag_ = true;
	}
	if (!expandTimer_->IsCounting())
	{
		state_ = State::Expand;
	}

	if (state_ == State::Expand)
	{
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());
		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->TakeAttack(damage_);
		}
	}

	if (owner_->EndPrison())
	{
		Kill();
	}
	
}
