#pragma once
#include "../../Actor.h"
class Chandelier : public Actor
{
	const float movementAmount_;
	const int damage_;
	public:
	Chandelier();
	virtual ~Chandelier() {};

	void Move();

	void HitPlayer();
};

