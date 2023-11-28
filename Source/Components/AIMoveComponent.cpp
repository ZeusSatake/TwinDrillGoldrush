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
	ML::Vec2 toVec = target->pos_ - owner_->pos_;

	float angle = atan2(-toVec.y, toVec.x);
	owner_->angle_ = angle;

	//owner_->pos_ += static_cast<Character*>(owner_)->GetMovement()->GetDirection() * static_cast<Character*>(owner_)->GetMovement()->GetSpeed();

}

void AIMoveComponent::Patroll(class Actor* target)
{
	if (!static_cast<NPC*>(owner_)->CheckFrontFoot())
	{
		if (owner_->angle_LR_ == Actor::Angle_LR::Left)
		{
			owner_->angle_LR_ = Actor::Angle_LR::Right;
			//static_cast<Character*>(owner_)->GetMovement()->SetMoveVecX(abs(static_cast<Character*>(owner_)->GetMovement()->GetMoveVec().x));
		}
		else
		{
			owner_->angle_LR_ = Actor::Angle_LR::Left;
			//static_cast<Character*>(owner_)->GetMovement()->SetMoveVecX(-abs(static_cast<Character*>(owner_)->GetMovement()->GetMoveVec().x));
		}
		
	}
}

void AIMoveComponent::Jump()
{


}

void AIMoveComponent::KnockBack()
{


}