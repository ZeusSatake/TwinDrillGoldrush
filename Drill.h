#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attackPoint;
	int   durability;
	float nowAngle;
	float Lenght;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Drill();
	virtual ~Drill() { };

	void SetAngle(float angle);

	int GetAttackPoint();
	float GetNowAngle();
	float UpdateDrillAngle();

	void Mining();
	void DrillCheckMove(ML::Vec2 e_);

	StateComponent::State dState;
	void Think()override;
	void Move()override;
};