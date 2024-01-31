#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attack;//�U����
	int   durability; //�ϋv�n
	int   maxDurability;
	float addAngle;   //�ǉ������p�x
	float angle;      //�p�x
	float Length;
	float preAngle;   //1�t���[���O�̊p�x
	bool  canRotate;  //��]�������邩�ǂ���
	bool CheckOverHeat;
	ML::Vec2 drawPos;
	ML::Vec2 animMove;
	ML::Vec2 targetPos;
public:
	enum class Mode{Normal,Drill,Non};
private:
	Mode mode;

	ML::Vec2 SearchBox[9] = {
	ML::Vec2{0,0},ML::Vec2{1,0},ML::Vec2{-1,0},
	ML::Vec2{0,-1},ML::Vec2{0,1},ML::Vec2{1,-1},
	ML::Vec2{1,1}
	};

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

	void SetCheckOverHeat(bool check);
	bool GetCheckOverHeat();

	void SetMode(Mode mode_);
	void UpdateMode(StateComponent::State state);
	Mode GetMode();

	ML::Vec2 GetDrawPos();
	ML::Vec2 GetTargetPos();

	ML::Vec2 DrillAngleVec();

	ML::Vec2 ChangeBrockPos();

	int GetAttack();
	void SetAttack(int attack_);
	float GetLenght();
	float GetNowAngle();
	float UpdateDrillAngle();

	void SetDurability(int num);//�Z�b�^�[
	void InitDurability(int num);//�������p

	int GetDurability();              //�ϋv�l���擾����
	int GetMaxDurability();           //�ő�ϋv�l�擾
	float GetNowDurabilityPercent();  //�ϋv�l�����擾

	void ResetDurability();

	void Mining();
	void Mining(ML::Vec2 pos);



	ML::Vec2 GetAttackPos();
	bool LimitLength(ML::Vec2 pos);
	void SearchBrocks(ML::Vec2 pos);
	void DrillCheckMove(ML::Vec2 e_);

	void setAnim();

	void SetAnimMove(ML::Vec2 move_);
	ML::Vec2 GetAnimMove();

	StateComponent::State dState;
};