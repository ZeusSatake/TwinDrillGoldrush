#include "PushButton.h"

PushButton::PushButton()
	:
	resetTime_sec_(1.0f)
{

}

void PushButton::OnEvent()
{

}
void PushButton::OffEvent()
{

}

const shared_ptr<Timer> PushButton::GetTimer() const
{
	return resetTimer_;
}

void PushButton::UpDate()
{
	ToggleButton::UpDate();

	if (IsPressed())
	{

	}
}