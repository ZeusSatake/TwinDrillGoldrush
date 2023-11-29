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
	
	void PlayerMove(ML::Vec2 vec);//�x�N�g��������Ƃ��̕��v���C���[�𓮂���

	StateComponent::State pState;
	
	void Think()override;
	void Move()override;
	void ResetCnt();

	//���łɊԂɍ��킹�邽�߂̐ڒn����
	bool CheckFoot();
	bool CheckHead();
	void CheckMove(ML::Vec2& e_);

	ML::Vec2 GetPos();
	ML::Vec2 GetMoveVec();
};