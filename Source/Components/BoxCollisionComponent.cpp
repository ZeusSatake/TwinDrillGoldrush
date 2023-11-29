#include "BoxCollisionComponent.h"
#include "../../Actor.h"

BoxCollisionComponent::BoxCollisionComponent(Actor* owner)
	:Component(owner)
{
	owner_ = owner;
}

template<class T>
T* BoxCollisionComponent::CheckHitObject(const ML::Box2D& hit) const
{
	if (CheckHit(hit))
	{
		return dynamic_cast<T*>(owner_);
	}
	return nullptr;
}
bool BoxCollisionComponent::CheckHit(const ML::Box2D& hit) const
{
	ML::Box2D me = hitBase_.OffsetCopy(owner_->GetPos());
	return me.Hit(hit);
}
ML::Box2D BoxCollisionComponent::getHitBase() const
{
	return hitBase_;
}
void BoxCollisionComponent::setHitBase(const ML::Box2D& hitBase)
{
	hitBase_ = hitBase;
}
