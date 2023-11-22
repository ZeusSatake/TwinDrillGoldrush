#pragma once
#include "Actor.h"

class ToggleButton : public Actor
{
	enum class ButtonState
	{
		IsPressed			= 1 << 0,
		IsSelected			= 1 << 1,
		ReceiveInput		= 1 << 2,
		InitExecuteEvent	= 1 << 3,
	};
	int buttonState_;
	XI::VGP enterButton_;

	void SetButtonState(const int bit, const bool flag);
	bool GetButtonState(const int bit) const;

	void ToggleEvent();
public:
	ToggleButton();
	virtual ~ToggleButton() {};

	virtual void OnEvent();
	virtual void OffEvent();

	void Reset();

	void SetSelected(const bool selected);
	void SetInitExecuteEvent(const bool initExecute);
	void SetRecieveInputEnable(const bool inputRecieve);
	bool IsPressed() const;
	bool IsSelected() const;

	void SetEnterButton(const XI::VGP enterButton);

	void UpDate() override;
};