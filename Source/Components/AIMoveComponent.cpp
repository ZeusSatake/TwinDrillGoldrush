#include "AIMoveComponent.h"


AIMoveComponent::AIMoveComponent(class Character* owner)
	: Component(owner)
	, moveVec_(0,0)
{
}

AIMoveComponent::AIMoveComponent(class Character* owner,float moveSpeed)
	: Component(owner)
	,moveVec_(0,0)
{
	static_cast<Character*>(owner_)->GetMovement()->SetSpeed(moveSpeed);
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

	owner_->pos_ += static_cast<Character*>(owner_)->GetMovement()->GetDirection() * static_cast<Character*>(owner_)->GetMovement()->GetSpeed();
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