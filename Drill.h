#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int attackPoint;
	int durability;
	float nowAngle;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Drill();
	virtual ~Drill() { };

	void InitPos(ML::Vec2 pos);//位置の初期化
	void SetAngle(float angle);

	int GetAttackPoint();
	float GetNowAngle();
	ML::Vec2 GetDrillPoint();
	float UpdateDrillAngle();

	StateComponent::State dState;
	void Think()override;
	void Move()override;
};