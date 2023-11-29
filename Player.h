#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"

class Player :public Character
{
	ML::Vec2 moveVec;
protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
public:
	Player();
	virtual ~Player() {}; 
	

	StateComponent::State pState;
	
	void Think()override;
	void Move()override;
	void ResetCnt();

	//ƒ¿”Å‚ÉŠÔ‚É‡‚í‚¹‚é‚½‚ß‚ÌÚ’n”»’è
	bool CheckFoot();
	bool CheckHead();
	void CheckMove(ML::Vec2& e_);

	ML::Vec2 GetMoveVec();
};