#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
class Drill :public Character
{
	int   attack;//攻撃力
	int   durability; //耐久地
	int   maxDurability;
	float addAngle;   //追加した角度
	float angle;      //角度
	float Length;
	float preAngle;   //1フレーム前の角度
	bool  canRotate;  //回転を許可するかどうか
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

	void SetDurability(int num);//セッター
	void InitDurability(int num);//初期化用

	int GetDurability();              //耐久値を取得する
	int GetMaxDurability();           //最大耐久値取得
	float GetNowDurabilityPercent();  //耐久値を％取得

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