#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attack;//çUåÇóÕ
	int   durability; //ëœãvín
	float addAngle;   //í«â¡ÇµÇΩäpìx
	float angle;      //äpìx
	float Length;
	float preAngle;   //1ÉtÉåÅ[ÉÄëOÇÃäpìx
	bool  canRotate;  //âÒì]Çãñâ¬Ç∑ÇÈÇ©Ç«Ç§Ç©
	ML::Vec2 drawPos;
	ML::Vec2 targetPos;
public:
	enum class Mode{Normal,Drill,Non};
private:
	Mode mode;

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

	void SetMode(StateComponent::State state);
	Mode GetMode();

	ML::Vec2 GetDrawPos();
	ML::Vec2 GetTargetPos();

	ML::Vec2 DrillAngleVec();

	ML::Vec2 ChangeBrockPos();

	int GetAttack();
	float GetLenght();
	float GetNowAngle();
	float UpdateDrillAngle();


	void Mining();
	void Mining(ML::Vec2 pos);

	ML::Vec2 SearchBox[9] = {
		ML::Vec2{0,0},ML::Vec2{1,0},ML::Vec2{-1,0},
		ML::Vec2{0,-1},ML::Vec2{0,1},ML::Vec2{1,-1},
		ML::Vec2{1,1}
	};

	ML::Vec2 GetAttackPos();
	bool LimitLength(ML::Vec2 pos);
	void SearchBrocks(ML::Vec2 pos);
	void DrillCheckMove(ML::Vec2 e_);


	StateComponent::State dState;
};