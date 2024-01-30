#include "FallObject.h"
#include "../Actors/Task_Player.h"

FallObject::FallObject()
	:Actor()
	, movementAmount_(1.5f)
	, damage_(20)
{

}

void FallObject::Move()
{
	SetMoveVecY(movementAmount_);

	SetPosY(GetPos().y + GetMoveVec().y);

	HitPlayer();
}

void FallObject::HitPlayer()
{
	ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
	plBox.Offset(ge->playerPtr->GetPos());
	if (box_->CheckHit(plBox))
	{
		ge->playerPtr->TakeAttack(damage_);
	}
}