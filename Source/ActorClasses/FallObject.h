#pragma once
#include "../../Actor.h"
class FallObject : public Actor
{
	float movementAmount_;
	int damage_;
public:
	FallObject();
	virtual ~FallObject() {};

	void Move();

	void HitPlayer();

	void SetDamage(int damage) { damage_ = damage; }
	void SetMovementAmount(float movementAmount) { movementAmount_ = movementAmount; }
};

