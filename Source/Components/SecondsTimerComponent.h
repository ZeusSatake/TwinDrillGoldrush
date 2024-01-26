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
	bool wasCountEnd_;		//�J�E���g�I����
	//�J�E���g��������
	void SetActive();
public:
	SecondsTimerComponent(class GameObject* owner);
	SecondsTimerComponent(class GameObject* ownerint, const float countSeconds);

	void SetCountSeconds(const float countSeconds);

	//���݂̃J�E���g�擾
	float  GetCount() const;
	//�J�E���g��
	bool IsActive() const;
	//�J�E���g�I���t���[���̂�true
	bool IsCountEndFrame() const;
	//�J�E���g�I����̂�true
	bool WasCountEnd() const;

	//�^�C�}�[�N��
	void Start();

	void Update() override;
};