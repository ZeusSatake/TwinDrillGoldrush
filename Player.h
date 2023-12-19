#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
#include "Source/Actors/Task_Drill.h"
#include "Source/Components/StatusComponent.h"

class Player :public Character
{
	ML::Vec2 moveVec;
	float speed;
	float jumpPow;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
	
public:
	Player();
	virtual ~Player() {}; 
	class shared_ptr<StatusComponent> status_;

	StateComponent::State pState;
	
	void Think()override;
	void Move()override;

	//Éøî≈Ç…ä‘Ç…çáÇÌÇπÇÈÇΩÇﬂÇÃê⁄ínîªíË
	bool CheckFoot();
	bool CheckHead();
	//void CheckMove(ML::Vec2& e_);
	void Dash(ML::Vec2 e_);

	ML::Vec2 GetMoveVec();
	shared_ptr<drill::Object> drill_;

};