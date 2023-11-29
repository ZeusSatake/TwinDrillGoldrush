#include "CircleCollisionComponent.h"

CircleCollisionComponent::CircleCollisionComponent(Actor* owner) 
	: Component(owner),
	radius_(0.0f)
{
}

void CircleCollisionComponent::setRadius(const float radius)
{
	radius_ = radius;
}

float CircleCollisionComponent::getRadius() const
{
	return radius_;
}

const ML::Vec2& CircleCollisionComponent::getCenter() const
{
	return owner_->GetPos();
}

bool CircleCollisionComponent::Intersect(const CircleCollisionComponent& other) const
{
	//‹——£‚Ì2æ‚ğ‹‚ß‚é
	ML::Vec2 diff = getCenter() - other.getCenter();
	float distSq = diff.Length() * diff.Length();

	//”¼Œa‚Ì˜a‚Ì2æ‚ğ‹‚ß‚é
	float radiiSq = getRadius() + other.getRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}