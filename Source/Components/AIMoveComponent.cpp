#include "AIMoveComponent.h"
#include "../../NPC.h"


AIMoveComponent::AIMoveComponent(class Character* owner)
	: Component(owner)
{
}

AIMoveComponent::AIMoveComponent(class Character* owner,float moveSpeed)
	: Component(owner)

{
	//static_cast<Character*>(owner_)->GetMovement()->SetSpeed(moveSpeed);
}

void AIMoveComponent::Update()
{

}

void AIMoveComponent::MoveTo(class Actor* target)
{
	ML::Vec2 toVec = target->GetPos() - owner_->GetPos();

	float angle = atan2(-toVec.y, toVec.x);
	owner_->angle_ = angle;

	//owner_->pos_ += static_cast<Character*>(owner_)->GetMovement()->GetDirection() * static_cast<Character*>(owner_)->GetMovement()->GetSpeed();

}

void AIMoveComponent::Patroll(class Actor* target)
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
		if (static_cast<NPC*>(owner_)->CheckFront()||!static_cast<NPC*>(owner_)->CheckFrontFoot())
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


}

void AIMoveComponent::KnockBack()
{


}