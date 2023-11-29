#include "TimerComponent.h"

//==============================================================
//コンストラクタ
//==============================================================
TimerComponent::TimerComponent(GameObject* owner)
	: Component((Actor*)owner),
	nowCount_(0),
	countFrame_(0),
	isCounting_(false)
{
};
TimerComponent::TimerComponent(class GameObject* owner,int count, int countFrame)
	: Component((Actor*)owner),
	nowCount_(count),
	countFrame_(countFrame)
{
	JudgeCount();
};

//==============================================================
//ゲッタ
//==============================================================
int TimerComponent::GetCount()
{
	return nowCount_;
}
bool TimerComponent::IsCounting()
{
	return this->isCounting_;
}

//==============================================================
//セッタ
//==============================================================
void TimerComponent::SetCountFrame(int countFrame)
{
	this->countFrame_ = countFrame;
}

//==============================================================
//イベント
//==============================================================
void TimerComponent::Start()
{
	if (countFrame_ <= 0)
		return;

	nowCount_ = countFrame_;
	isCounting_ = true;
}
void TimerComponent::Update()
{
	if (nowCount_ < 0)
		return;

	--nowCount_;

	JudgeCount();
}
void TimerComponent::JudgeCount()
{
	if (nowCount_ <= 0) {
		isCounting_ = false;
		nowCount_ = 0;
	}
	else if (!isCounting_) {
		isCounting_ = true;
	}
}