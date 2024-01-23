#pragma once
#include "../../Actor.h"
#include "../../Source/Components/TimerComponent.h"
class SobaLaser : public Actor
{
	float movementAmount_;
	bool countFlag_;
	bool initFlag_;
	int damage_;
	shared_ptr<TimerComponent> suviveTimer_;
public:
	SobaLaser();
	virtual ~SobaLaser() {};

	void Move();

	enum Direction
	{
		Down,
		Left,
		Right
	};
private: 		
	Direction direction_;
public:
	void Iniitialize();
	Direction GetDirection() { return direction_; }
	void SetDirection(Direction direction) { direction_ = direction; }
	void SetMovementAmount(const float movementAmount) { movementAmount_ = movementAmount; }
	void MoveByDirection();
};

