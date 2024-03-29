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
	countSeconds_(countFrame),
	isActive_(false),
	isCountEndFrame_(false),
	wasCountEnd_(false)
{
	SetActive();
};

//==============================================================
//ゲッタ
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
bool SecondsTimerComponent::WasCountEnd() const
{
	return wasCountEnd_;
}

//==============================================================
//セッタ
//==============================================================
void SecondsTimerComponent::SetCountSeconds(const float countSeconds)
{
	if (countSeconds < 0.0f)
		assert(!"カウントする秒数にマイナスの値は入れないでください。");
	countSeconds_ = countSeconds;
}

//タイマー起動
void SecondsTimerComponent::Start()
{
	if (countSeconds_ <= 0)
		return;

	nowCount_ = countSeconds_;
	isActive_ = true;
	wasCountEnd_ = false;
}

void SecondsTimerComponent::Stop()
{
	isActive_ = false;
}

void SecondsTimerComponent::Update()
{
	if (!isActive_)
	{
		isCountEndFrame_ = false;
		return;
	}

	nowCount_ -= 1.0f / REFRESHRATE;

	if (nowCount_ <= 0.0f)
	{
		isCountEndFrame_ = true;
		wasCountEnd_ = true;
	}

	SetActive();
}
void SecondsTimerComponent::SetActive()
{
	isActive_ = nowCount_ > 0.0f;

	if (!isActive_ && nowCount_ < 0.0f)
		nowCount_ = 0.0f;
}