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
	float Length;
	float preAngle;   //1�t���[���O�̊p�x
	bool  canRotate;  //��]�������邩�ǂ���
	ML::Vec2 drawPos;
	ML::Vec2 targetPos;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Drill();
	virtual ~Drill() { };

	ML::Vec2 moveVec;
	ML::Vec2 plPos;


	void SetAngle(float angle);
	void SetDrawPos(ML::Vec2 pos);
	void UpdateTargetPos(ML::Vec2 pos);
	void UpdateLength(float length);
	bool SpinAngle(float angle);
	void SetCanRotate(bool check);

	ML::Vec2 GetDrawPos();
	ML::Vec2 GetTargetPos();
	ML::Vec2 DrillAngleVec();
	ML::Vec2 ChangeBrockPos();
	int GetAttackPoint();
	float GetLenght();
	float GetNowAngle();
	float UpdateDrillAngle();

	void Mining();
	bool LimitLength(ML::Vec2 pos);
	void SearchBrocks(ML::Vec2 pos);
	void DrillCheckMove(ML::Vec2 e_);

	StateComponent::State dState;
};