#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
#include "Source/Components/TimerComponent.h"
#include "Source/Actors/Task_Drill.h"
#include "Source/Components/StatusComponent.h"

namespace DrawGauge { class Object; }
class HPBarComponent;//includeすると循環参照が起こるため前方宣言
namespace Save { class Object; }

class Player :public Character
{
	ML::Vec2 moveVec;
	ML::Vec2 externalMoveVec;
	ML::Box2D AttackBox;
	float jumpPow;
	bool extCheckFoot;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
	class shared_ptr<TimerComponent> cooldown,overheat;
	class shared_ptr<StatusComponent>status_;
	shared_ptr<HPBarComponent> hpBar_;
	shared_ptr<Save::Object> save_;
public:
	Player();
	virtual ~Player() {}; 


	StateComponent::State pState;

private:
	int hpTable[5] = { 1000,1500,1700,2000,2300 };
	int defenceTable[5] = { 1,3,5,7,9 };
	int attackTable[5] = { 3,5,10,15,20 };
	float speedTable[5] = {1.5f,1.7f,2.0f,2.3f,2.5f};

	int drillAttack[5] = { 1,1,2,2,3 };


public:
	
	void Think()override;
	void Move()override;

	//α版に間に合わせるための接地判定
	bool CheckFoot();

	void SetExtCheckFoot(bool check_);
	bool GetExtCheckFoot();
	void ExtCheckFoot(ML::Box2D target_);
	

	bool CheckHead();
	//void CheckMove(ML::Vec2& e_);
	void Dash(ML::Vec2 e_);

	void HiddenPlayer();

	void HitAttack(); //攻撃が当たった時の処理
	void TakeAttack(int damage_); //攻撃を受けたときの処理

	void SetPlayerState(StateComponent::State state); //ステートの外部変更
	void SetExternalVec(ML::Vec2 moveVec_);//外部から渡された移動量で移動するようのメソッド 
	ML::Vec2 GetExternalVec();

	void ResetState();

	bool UpdateDrilldurability();
	void SetDrillDurability(int durability_);
	int  GetDrilldurability();

	void UpdateStates();//プレイヤーのステータスを一括でアップデートするメソッド

	int GetDrillAttack();
	ML::Box2D GetAttackBox();
	ML::Vec2 GetMoveVec();

	StatusComponent* GetStatus() const;
	HPBarComponent* GetHPBar() const;
    
	void CollisionJudge(ML::Box2D hitBox_ , ML::Vec2 pos_);//外部のオブジェクトにめり込まないようにするメソッド


	void SetAnim();

	shared_ptr<drill::Object> drill_;
	shared_ptr<DrawGauge::Object> hpGauge_;
};