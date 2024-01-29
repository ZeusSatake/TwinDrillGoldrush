#include "Fish.h"
#include "../Actors/Task_Player.h"

Fish::Fish()
	:Enemy()
	,movementAmount_(3.f)
	,limitRange_(750)
{
	GetStatus()->attack.Initialize(10, 100);
	SetTarget(ge->playerPtr.get());
}

void Fish::Move()
{
	SetMoveVecX(-movementAmount_);

	SetPosX(GetPos().x + GetMoveVec().x);

	limitRange_ -= movementAmount_;
	//ƒ_ƒ[ƒW‚ð—^‚¦‚éˆ—
	ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
	plBox.Offset(GetTarget()->GetPos());
	if (box_->CheckHit(plBox))
	{
		HitPlayer();
		Kill();
	}

	if (limitRange_ <= 0)
	{
		Kill();
	}
}

void Fish::HitPlayer()
{
	static_cast<Player*>(GetTarget())->TakeAttack(status_->attack.GetNow());
}


