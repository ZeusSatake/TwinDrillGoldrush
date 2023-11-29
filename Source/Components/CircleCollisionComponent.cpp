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
	//������2������߂�
	ML::Vec2 diff = getCenter() - other.getCenter();
	float distSq = diff.Length() * diff.Length();

	//���a�̘a��2������߂�
	float radiiSq = getRadius() + other.getRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}