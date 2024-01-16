#include "Chandelier.h"
#include "../Actors/Task_Player.h"

Chandelier::Chandelier()
	:Actor()
	, movementAmount_(2.5f)
	, damage_(100)
{

}

void Chandelier::Move()
{
	SetMoveVecY(movementAmount_);
	
	SetPosY(GetPos().y + GetMoveVec().y);

	HitPlayer();
}

void Chandelier::HitPlayer()
{
	if (ge->playerPtr->pState != StateComponent::State::Damage)
	{
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());

		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->GetStatus()->HP.TakeDamage(damage_);
		}
	}
}
