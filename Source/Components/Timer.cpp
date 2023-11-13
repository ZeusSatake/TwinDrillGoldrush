#include "Timer.h"

//==============================================================
//�R���X�g���N�^
//==============================================================
Timer::Timer(GameObject* owner)
	: Component((Actor*)owner),
	nowCount_(0),
	countFrame_(0),
	isCounting_(false)
{
};
Timer::Timer(class GameObject* owner,int count, int countFrame)
	: Component((Actor*)owner),
	nowCount_(count),
	countFrame_(countFrame)
{
	JudgeCount();
};

//==============================================================
//�Q�b�^
//==============================================================
int Timer::GetCount()
{
	return nowCount_;
}
bool Timer::IsCounting()
{
	return this->isCounting_;
}

//==============================================================
//�Z�b�^
//==============================================================
void Timer::SetCountFrame(int countFrame)
{
	this->countFrame_ = countFrame;
}

//==============================================================
//�C�x���g
//==============================================================
void Timer::Start()
{
	if (countFrame_ <= 0)
		return;

	nowCount_ = countFrame_;
	isCounting_ = true;
}
void Timer::Update()
{
	if (nowCount_ < 0)
		return;

	--nowCount_;

	JudgeCount();
}
void Timer::JudgeCount()
{
	if (nowCount_ <= 0) {
		isCounting_ = false;
		nowCount_ = 0;
	}
	else if (!isCounting_) {
		isCounting_ = true;
	}
}