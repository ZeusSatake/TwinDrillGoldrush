#include "PushButton.h"
#include "../Components/SecondsTimerComponent.h"

PushButton::PushButton()
{
	AddComponent(resetTimer_ = make_shared<SecondsTimerComponent>(this));
	SetResetTime(1.0f);
	SetEnable_ResetTimer(true);
}

void PushButton::OnEvent()
{

}
void PushButton::OffEvent()
{

}

void PushButton::SetResetTime(const float resetTime_sec)
{
	resetTime_sec_ = resetTime_sec;
	resetTimer_->SetCountSeconds(resetTime_sec_);
}

const shared_ptr<SecondsTimerComponent> PushButton::GetTimer() const
{
	return resetTimer_;
}

void PushButton::UpDate()
{
	if (resetCount > 0 && ge->mouse->CheckBT(XI::Mouse::MB::LB) == XI::Mouse::ButtonState::Down)
	{
		int a = 0;
	}

	ToggleButton::UpDate();

	if (enableResetTimer_)
	{
		//���Z�b�g
		if (resetTimer_->IsCountEndFrame())
		{
			++resetCount;
			Reset();
			SetRecieveInputEnable(true);
		}

		//���Z�b�g�J�E���g�J�n
		if (IsPressed() && !resetTimer_->IsActive())
		{
			resetTimer_->Start();
			//������Ă���Ԃ͓��͂��󂯕t���Ȃ�
			SetRecieveInputEnable(false);
		}

		resetTimer_->Update();
	}
}

void PushButton::SetEnable_ResetTimer(const bool stop)
{
	enableResetTimer_ = stop;
}