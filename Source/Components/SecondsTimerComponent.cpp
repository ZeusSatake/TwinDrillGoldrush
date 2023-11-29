#include "SecondsTimerComponent.h"

//==============================================================
//コンストラクタ
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
	countSeconds_(countFrame)
{
	SetActive();
};

//==============================================================
//ゲッタ
//==============================================================
int SecondsTimerComponent::GetCount() const
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
//セッタ
//==============================================================
void SecondsTimerComponent::SetCountSeconds(const float countSeconds)
{
	countSeconds_ = countSeconds;
}

//タイマー起動
void SecondsTimerComponent::Start()
{
	if (countSeconds_ <= 0)
		return;

	nowCount_ = countSeconds_;
	isActive_ = true;
}

void SecondsTimerComponent::Update()
{
	if (IsActive() == false)
		return;

	nowCount_ -= 1.0f / REFRESHRATE;

	SetActive();
}
void SecondsTimerComponent::SetActive()
{
	isCountEndFrame_ = isActive_ && nowCount_ < 0;

	isActive_ = nowCount_ > 0;

	if (!isActive_ && nowCount_ < 0)
		nowCount_ = 0;
}