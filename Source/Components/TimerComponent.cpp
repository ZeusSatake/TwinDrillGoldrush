#include "TimerComponent.h"

//==============================================================
//�R���X�g���N�^
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
//�Q�b�^
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
//�Z�b�^
//==============================================================
void TimerComponent::SetCountFrame(int countFrame)
{
	this->countFrame_ = countFrame;
}

//==============================================================
//�C�x���g
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