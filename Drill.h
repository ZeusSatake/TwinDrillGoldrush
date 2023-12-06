#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attackPoint;//攻撃力
	int   durability; //耐久地
	float addAngle;   //追加した角度
	float angle;      //角度
	float preAngle;   //1フレーム前の角度
	bool  canRotate;  //回転を許可するかどうか

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Drill();
	virtual ~Drill() { };

	void SetAngle(float angle);
	bool SpinAngle(float angle);
	void SetCanRotate(bool check);

	int GetAttackPoint();
	float GetNowAngle();
	ML::Vec2 DrillAngleVec();
	float UpdateDrillAngle();

	void Mining();
	void SearchBrocks(ML::Vec2 pos_);
	void DrillCheckMove(ML::Vec2 e_);

	StateComponent::State dState;
};