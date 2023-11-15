#include "AIMoveComponent.h"

AIMoveComponent::AIMoveComponent(class Character* owner)
	: Component((Actor*)owner)
	,moveVec_(owner->GetMovement()->GetVelocity())
	,moveSpeed_(owner->GetMovement()->GetSpeed())
{
}

AIMoveComponent::AIMoveComponent(class Character* owner,ML::Vec2 moveVec,float moveSpeed)
	: Component((Actor*)owner)
	,moveVec_(moveVec)
	,moveSpeed_(moveSpeed)
{
}

void AIMoveComponent::Update()
{
}

void AIMoveComponent::MoveTo(class Actor* target)
{
	
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