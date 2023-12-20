#pragma once
#include"../../Component.h"
//-----------------------------------------------------------------------
// �X�e�[�g�}�V��
//-----------------------------------------------------------------------
class StateComponent : public Component
{
public:
	enum class State
	{
		Non,        //����
		Idle,		//�ҋ@
		Walk,		//����
		Attack,		//�U��
		SpinAttack, //��]�U��
		Damage,	    //�_���[�W
		KnockBack,  //�̂�����
		Dead,		//���S
		Jump,       //�W�����v
		Fall,       //�~��
		Dash,       //�_�b�V��
		Drill,      //�h����
		DrillDash,  //�h�����_�b�V��
		Mining,     //�̌@
		Appeal      //�A�s�[��
	};
	int moveCnt_;
private:
	State nowState;
	State preState;
public:
	StateComponent(class Actor* actor);
	void UpdateNowState(State state); //�X�e�[�^�X�̍X�V
	void InitializeState(State state);//�X�e�[�^�X�̏�����
	void InitializeState();
	State GetNowState();              //nowState�̃Q�b�^�[

};