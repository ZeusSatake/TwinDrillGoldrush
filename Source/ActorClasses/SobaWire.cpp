#include "SobaWire.h"
#include "../Actors/Task_Player.h"

SobaWire::SobaWire()
{
	box_->setHitBase(ML::Box2D{ 0, 0, 2600,16 });
	for (int i = 0; i < 3; ++i)
	{
		weakPoint_.push_back(shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
		AddComponent(weakPoint_[i]);
		weakPoint_[i]->setHitBase(ML::Box2D{ 0, 0, 32, 16 });
	}
	

	//—\‚ß3‚Â‚Ìã“_‚ğì‚Á‚Ä‚¨‚­
	for (int i = 0; i < 3; ++i)
	{
		weakPointPos_.push_back(ML::Vec2{ 0, 0 });
	}
}

void SobaWire::Update()
{
	for (int i=0;i<weakPoint_.size();++i)
	{
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());
		ML::Box2D ownBox = weakPoint_[i]->getHitBase();
		ownBox.Offset(weakPointPos_[i]);
		if (ge->playerPtr->pState == StateComponent::State::Attack)
		{
			if (ownBox.Hit(plBox))
			{
				owner_->PatternSwitch();
				owner_->EndAttack();
				owner_->DestroyedWire();
				Kill();
			}
		}
	}
	if (owner_->EndWire())
	{
		Kill();
	}
}

