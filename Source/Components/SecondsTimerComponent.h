#pragma once
#include "../../Component.h"
#include "../../GameObject.h"

//�J�E���g���J�n�������ꏊ��Timer��Start()���Ăяo��
//IsActive()��true�̎��̓J�E���g��
class SecondsTimerComponent : public Component
{
	float nowCount_;		//���ԃJ�E���^
	float countSeconds_;	//�J�E���g����b��
	bool isActive_;			//�J�E���g��
	bool isCountEndFrame_;	//�J�E���g�I���t���[��
	//�J�E���g��������
	void SetActive();
public:
	SecondsTimerComponent(class GameObject* owner);
	SecondsTimerComponent(class GameObject* ownerint, const float nowCount, const float countSeconds);

	void SetCountSeconds(const float countSeconds);

	//���݂̃J�E���g�擾
	int  GetCount() const;
	//�J�E���g��
	bool IsActive() const;
	bool IsCountEndFrame() const;
	//���G���ԊJ�n
	void Start();
	//�X�V
	void Update() override;
};