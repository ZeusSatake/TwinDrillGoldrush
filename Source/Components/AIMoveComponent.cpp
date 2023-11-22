#include "AIMoveComponent.h"


AIMoveComponent::AIMoveComponent(class Character* owner)
	: Movement(owner)
	,moveVec_(owner->GetMovement()->GetVelocity())
{
}

AIMoveComponent::AIMoveComponent(class Character* owner,ML::Vec2 moveVec,float moveSpeed)
	: Movement(owner,moveSpeed)
	,moveVec_(moveVec)
{
}

void AIMoveComponent::Update()
{

}

void AIMoveComponent::MoveTo(class Actor* target)
{
	//‘Š‘ÎÀ•W‚ð‹‚ß‚é
	ML::Vec2 toVec = target->pos_ - owner_->pos_;

	float angle = atan2(-toVec.y, toVec.x);
	owner_->angle_ = angle;

	owner_->pos_ += static_cast<Character*>(owner_)->GetMovement()->GetDirection()* static_cast<Character*>(owner_)->GetMovement()->GetSpeed();
}

void AIMoveComponent::Patroll(class Actor* target)
{

}

void AIMoveComponent::Jump()
{


}

void AIMoveComponent::KnockBack()
{


}