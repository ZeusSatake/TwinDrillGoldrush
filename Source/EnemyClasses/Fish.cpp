#include "Fish.h"
#include "../Actors/Task_Player.h"

Fish::Fish()
	:Enemy()
	,movementAmount_(3.f)
	,limitRange_(750)
{

	//GetStatus()->HP.SetMaxHP(10,StatusComponent::Health::MaxLifeSetMode::MaxHeal);
	//GetStatus()->attack.SetMax(10);
	SetTarget(ge->playerPtr.get());
}

void Fish::Move()
{
	SetMoveVecX(-movementAmount_);

	SetPosX(GetPos().x + GetMoveVec().x);

	limitRange_ -= movementAmount_;
	//�_���[�W��^���鏈��
	if (static_cast<Player*>(GetTarget())->pState != StateComponent::State::Damage)
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		if (box_->CheckHit(plBox))
		{
			HitPlayer();
			Kill();
		}
	}
	
	if (limitRange_ <= 0)
	{
		Kill();
	}
}

void Fish::HitPlayer()
{
	static_cast<Player*>(GetTarget())->GetStatus()->HP.TakeDamage(status_->attack.GetNow());
}


