#pragma once
#include "../../Component.h"
#include "../../myLib.h"
#include "../../Actor.h"
class CircleCollisionComponent : public Component
{
	float radius_;
public:
	CircleCollisionComponent(class Actor* owner);
	virtual ~CircleCollisionComponent() {};

	void setRadius(const float radius);
	float getRadius() const;

	const ML::Vec2& getCenter() const;

	//���̉~�Ƃ̌�������(���s���m�F���Ă��Ȃ����ߗv����)
	bool Intersect(const CircleCollisionComponent& other) const;
};

