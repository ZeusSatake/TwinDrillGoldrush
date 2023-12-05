#include "PushButton.h"
#include "../Components/SecondsTimerComponent.h"

PushButton::PushButton()
{
	AddComponent(resetTimer_ = make_shared<SecondsTimerComponent>(this));
	SetResetTime(1.0f);
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
	//リセット
	if (resetTimer_->IsCountEndFrame())
	{
		++resetCount;
		Reset();
		SetRecieveInputEnable(true);
	}	

	//リセットカウント開始
	if (IsPressed() && !resetTimer_->IsActive())
	{
		resetTimer_->Start();
		//押されている間は入力を受け付けない
		SetRecieveInputEnable(false);
	}


	resetTimer_->Update();
}