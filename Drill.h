#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attackPoint;//�U����
	int   durability; //�ϋv�n
	float addAngle;   //�ǉ������p�x
	float angle;      //�p�x
	float preAngle;   //1�t���[���O�̊p�x
	bool  canRotate;  //��]�������邩�ǂ���

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