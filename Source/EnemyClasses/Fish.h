#pragma once
#include "../../Enemy.h"

class Fish : public Enemy
{
	const float movementAmount_;
	float limitRange_;
public:
	Fish();
	virtual ~Fish() {};

	void Move() override;

	void HitPlayer();
};