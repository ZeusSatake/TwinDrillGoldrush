#include "ToggleButton.h"

ToggleButton::ToggleButton()
	:
	buttonState_(0)
{
}

void ToggleButton::OnEvent()
{

}
void ToggleButton::OffEvent()
{

}

void ToggleButton::ToggleEvent()
{
	buttonState_ ^= (int)ButtonState::IsPressed;
	
	if (GetButtonState((int)ButtonState::IsPressed))
		OnEvent();
	else
		OffEvent();
}

void ToggleButton::Reset()
{
	if (IsPressed())
	{
		ToggleEvent();
	}
}

void ToggleButton::SetButtonState(const int bit, const bool flag)
{
	if (flag)
		buttonState_ |= (int)bit;
	else
		buttonState_ &= ~(int)bit;
}
bool ToggleButton::GetButtonState(const int bit) const
{
	return (buttonState_ & bit) == bit;
}

void ToggleButton::SetRecieveInputEnable(const bool inputRecieve)
{
	SetButtonState((int)ButtonState::ReceiveInput, inputRecieve);
}
void ToggleButton::SetSelected(const bool selected)
{
	SetButtonState((int)ButtonState::IsSelected, selected);
}
void ToggleButton::SetInitExecuteEvent(const bool initExecute)
{
	SetButtonState((int)ButtonState::InitExecuteEvent, initExecute);
}

bool ToggleButton::IsPressed() const
{
	return GetButtonState((int)ButtonState::IsPressed);
}
bool ToggleButton::IsSelected() const
{
	return GetButtonState((int)ButtonState::IsSelected);
}

void ToggleButton::SetEnterButton(const XI::VGP enterButton)
{
	enterButton_ = enterButton;
}

void ToggleButton::UpDate()
{
	if (GetButtonState((int)ButtonState::IsSelected | (int)ButtonState::ReceiveInput))
	{
		if (ge->in1->CheckBT(enterButton_) == XI::GamePad::ButtonState::Down)
		{
			ToggleEvent();
		}
	}
}