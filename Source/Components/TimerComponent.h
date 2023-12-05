#pragma once
#include "../../Component.h"
#include "../../GameObject.h"
//�J�E���g���J�n�������ꏊ��Timer��Start()���Ăяo��
//�^�X�N��Update���Ȃǂ�Timer��Update()���Ăяo��
class TimerComponent : public Component
{
	int nowCount_;		//���ԃJ�E���^
	int countFrame_;	//�J�E���g����t���[��
	bool isCounting_;	//�J�E���g��
	//�J�E���g��������
	void JudgeCount();
public:
	TimerComponent(class GameObject* owner);
	TimerComponent(class GameObject* ownerint, int nowCount, int countFrame);

	//���݂̃J�E���g�擾
	int  GetCount();
	//�J�E���g��
	bool IsCounting();
	//�J�E���g����t���[������ݒ�
	void SetCountFrame(int countFrame);
	//���G���ԊJ�n
	void Start();
	//�X�V
	void Update();
};