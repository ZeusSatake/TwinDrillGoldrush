#pragma once
#include"../../Component.h"
//-----------------------------------------------------------------------
// �X�e�[�g�}�V��
//-----------------------------------------------------------------------
class StateComponent : public Component
{
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
		DrillDash,  //�h�����_�b�V��
		Mining,     //�̌@
		Appeal      //�A�s�[��
	};
	State nowState;
	State preState;
public:
	StateComponent(class Actor* actor);
	void UpdateNowState(State state); //�X�e�[�^�X�̍X�V
	void InitializeState(State state);//�X�e�[�^�X�̏�����
	State GetNowState();              //nowState�̃Q�b�^�[

};