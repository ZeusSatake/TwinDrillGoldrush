#include "SecondsTimerComponent.h"

//==============================================================
//�R���X�g���N�^
//==============================================================
SecondsTimerComponent::SecondsTimerComponent(GameObject* owner)
	:
	SecondsTimerComponent(owner, 0.0f)
{
};
SecondsTimerComponent::SecondsTimerComponent(GameObject* owner, const float countFrame)
	:
	Component((Actor*)owner),
	nowCount_(0.0f),
	countSeconds_(countFrame),
	isActive_(false),
	isCountEndFrame_(false)
{
	SetActive();
};

//==============================================================
//�Q�b�^
//==============================================================
float SecondsTimerComponent::GetCount() const
{
	return nowCount_;
}
bool SecondsTimerComponent::IsActive() const
{
	return isActive_;
}
bool SecondsTimerComponent::IsCountEndFrame() const
{
	return isCountEndFrame_;
}

//==============================================================
//�Z�b�^
//==============================================================
void SecondsTimerComponent::SetCountSeconds(const float countSeconds)
{
	if (countSeconds < 0.0f)
		assert(!"�J�E���g����b���Ƀ}�C�i�X�̒l�͓���Ȃ��ł��������B");
	countSeconds_ = countSeconds;
}

//�^�C�}�[�N��
void SecondsTimerComponent::Start()
{
	if (countSeconds_ <= 0)
		return;

	nowCount_ = countSeconds_;
	isActive_ = true;
}

void SecondsTimerComponent::Update()
{
	if (!isActive_)
	{
		isCountEndFrame_ = false;
		return;
	}

	nowCount_ -= 1.0f / REFRESHRATE;

	isCountEndFrame_ = nowCount_ <= 0.0f;

	SetActive();
}
void SecondsTimerComponent::SetActive()
{
	isActive_ = nowCount_ > 0.0f;

	if (!isActive_ && nowCount_ < 0.0f)
		nowCount_ = 0.0f;
}