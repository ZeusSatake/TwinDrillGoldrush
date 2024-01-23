#include "Sobanomi.h"
#include "../Actors/Task_Player.h"

Sobanomi::Sobanomi()
	:Actor()
	, fallSpeed_(2.5f)
	, damage_(30)
{
	box_->setHitBase(ML::Box2D{ -8, -8, 16, 16 });
}

void Sobanomi::Update()
{
	ML::Vec2 est;
	SetMoveVecY(fallSpeed_);

	ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
	plBox.Offset(ge->playerPtr->GetPos());
	if (box_->CheckHit(plBox))
	{
		ge->playerPtr->TakeAttack(damage_);
		Kill();
	}
	if (owner_->EndWire())
	{
		Kill();
	}

	est += GetMoveVec();
	CheckMove(est);
}