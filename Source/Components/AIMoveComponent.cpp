#include "AIMoveComponent.h"
#include "../../Enemy.h"


AIMoveComponent::AIMoveComponent(class Character* owner)
	: Component(owner)
{
}
	

void AIMoveComponent::Update()
{
}

void AIMoveComponent::MoveTo(class Actor* target)
{
	//‰¼ŽÀ‘•
	if (target->GetPos().x <= owner_->GetPos().x)
	{
		owner_->angle_LR_ = Actor::Angle_LR::Right;
		owner_->SetMoveVecX(1);
	}
	else
	{
		owner_->angle_LR_ = Actor::Angle_LR::Left;
		owner_->SetMoveVecX(-1);
	}
}

void AIMoveComponent::Patroll()
{
	if (static_cast<Character*>(owner_)->CheckFoot())
	{
		if (owner_->angle_LR_ == Actor::Angle_LR::Left)
		{
			owner_->SetMoveVecX(-1);
		}
		else
		{
			owner_->SetMoveVecX(1);
		}
		if (static_cast<NPC*>(owner_)->CheckFront())
		{
			if (owner_->angle_LR_ == Actor::Angle_LR::Left)
			{
				owner_->angle_LR_ = Actor::Angle_LR::Right;
			}
			else
			{
				owner_->angle_LR_ = Actor::Angle_LR::Left;
			}
		}
	}
}

void AIMoveComponent::Jump()
{
	owner_->SetMoveVecY(-1.f);
}

void AIMoveComponent::KnockBack()
{


}