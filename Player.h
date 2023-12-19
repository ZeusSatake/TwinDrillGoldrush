#pragma once
#include "Character.h"
#include "Source/Components/StateComponent.h"
#include "Source/Components/ControllerInputComponent.h"
#include "Source/Components/TimerComponent.h"
#include "Source/Actors/Task_Drill.h"

class Player :public Character
{
	ML::Vec2 moveVec;
	float speed;
	float jumpPow;

protected:
	class shared_ptr<ControllerInputComponent> controller_;
	class shared_ptr<StateComponent> state_;
	class shared_ptr<TimerComponent> cooldown_;
public:
	Player();
	virtual ~Player() {}; 

	StateComponent::State pState;
	
	void Think()override;
	void Move()override;

	//���łɊԂɍ��킹�邽�߂̐ڒn����
	bool CheckFoot();
	bool CheckHead();
	//void CheckMove(ML::Vec2& e_);
	void Dash(ML::Vec2 e_);

	void HitAttack(); //�U���������������̏���
	void TakeAttack(); //�U�����󂯂��Ƃ��̏���

	void SetPlayerState(StateComponent::State state); //�X�e�[�g�̊O���ύX

	ML::Vec2 GetMoveVec();
	shared_ptr<drill::Object> drill_;

};