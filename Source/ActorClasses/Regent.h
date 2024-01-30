#pragma once
#include "../../Actor.h"
#include "../../Source/Components/TimerComponent.h"
class Regent : public Actor
{
	float movementAmount_;
	bool countFlag_;
	bool initFlag_;
	int damage_;
	shared_ptr<TimerComponent> suviveTimer_;
public:
	Regent();
	virtual ~Regent() {};

	void Move();
public:
	void Iniitialize();
	void SetMovementAmount(const float movementAmount) { movementAmount_ = movementAmount; }
	void MoveByDirection();
	void SetDamage(const int damage) { damage_ = damage; }
};

