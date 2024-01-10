#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
#include "Source/Components/TimerComponent.h"
#include "Source/Actors/Task_Drill.h"
#include "Source/Components/StatusComponent.h"

class Player :public Character
{
	ML::Vec2 moveVec;
	ML::Box2D AttackBox;
	float jumpPow;
	int PreHp;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
	class shared_ptr<TimerComponent> cooldown_;
	class shared_ptr<StatusComponent>status_;
public:
	Player();
	virtual ~Player() {}; 


	StateComponent::State pState;
	
	void Think()override;
	void Move()override;

	//α版に間に合わせるための接地判定
	bool CheckFoot();
	bool CheckHead();
	//void CheckMove(ML::Vec2& e_);
	void Dash(ML::Vec2 e_);

	void HiddenPlayer();

	void HitAttack(); //攻撃が当たった時の処理
	void TakeAttack(); //攻撃を受けたときの処理

	void SetPlayerState(StateComponent::State state); //ステートの外部変更

	void ResetState();

	ML::Box2D GetAttackBox();
	ML::Vec2 GetMoveVec();
	StatusComponent* GetStatus() const;

	shared_ptr<drill::Object> drill_;

};