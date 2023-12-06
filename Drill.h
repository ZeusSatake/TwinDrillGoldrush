#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attackPoint;
	int   durability;
	float angle;
	float preAngle;
	bool  canRotate;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Drill();
	virtual ~Drill() { };

	void SetAngle(float angle);
	void SetCanRotate(bool check);

	int GetAttackPoint();
	float GetNowAngle();
	float UpdateDrillAngle();

	void Mining();
	void DrillCheckMove(ML::Vec2 e_);

	StateComponent::State dState;
};