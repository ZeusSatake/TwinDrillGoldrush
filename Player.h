#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"

class Player :public Character
{
protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Player();
	virtual ~Player() {}; 
	
	void PlayerMove(ML::Vec2 vec);//ベクトルを入れるとその分プレイヤーを動かす

	StateComponent::State pState;
	
	void Think()override;
	void Move()override;

	ML::Vec2 GetPos();
	ML::Vec2 Gravity();
};