#include "Chandelier.h"
#include "../Actors/Task_Player.h"

Chandelier::Chandelier()
	:Actor()
	, movementAmount_(2.5f)
	, damage_(20)
{

}

void Chandelier::Move()
{
	SetMoveVecY(movementAmount_);
	
	SetPosY(GetPos().y + GetMoveVec().y);

	HitPlayer();

	if (GetPos().y>ge->screen2DHeight)
	{
		Kill();
	}
}

void Chandelier::HitPlayer()
{
	ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
	plBox.Offset(ge->playerPtr->GetPos());

	if (box_->CheckHit(plBox))
	{
		ge->playerPtr->TakeAttack(damage_);
	}
}
