#pragma once
#include "../../Component.h"
#include "../../MyPG.h"
#include "../../XI2018_Ver1_0.h"

class ControllerInputComponent :public Component //�R���g���[���[�̓��͏��֌W
{
public:
	XI::GamePad::SP gamePad_;
	ControllerInputComponent(class Actor* actor);

	void SetGamePadId(XI::GamePad::SP controller_);//�ǂ̃R���g���[���[���������߂�Z�b�^�[

	float GetAngleLStick();                        //���X�e�B�b�N�̊p�x�����o��
	float GetAngleRStick();                        //�E�X�e�B�b�N�̊p�x�����o��

	ML::Vec2 GetLStickVec();
	ML::Vec2 GetRStickVec();

};