#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attackPoint;//UŒ‚—Í
	int   durability; //‘Ï‹v’n
	float addAngle;   //’Ç‰Á‚µ‚½Šp“x
	float angle;      //Šp“x
	float preAngle;   //1ƒtƒŒ[ƒ€‘O‚ÌŠp“x
	bool  canRotate;  //‰ñ“]‚ğ‹–‰Â‚·‚é‚©‚Ç‚¤‚©

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