#pragma once
#include "Actor.h"

class ToggleButton : public Actor
{
	enum class ButtonState
	{
		IsPressed			= 1 << 0,
		IsSelected			= 1 << 1,
		RecieveInput		= 1 << 2,
		InitExecuteEvent	= 1 << 3,
	};
	int buttonState_;
	XI::VGP enterButton_;
	XI::Mouse::MB mouseEnterButton_;
	string text_;
	string stateText_;

	void SetButtonState(const int bit, const bool flag);
	bool GetButtonState(const int bit) const;

	void SetStateText();

	void ToggleEvent();
public:
	ToggleButton();
	virtual ~ToggleButton() {};

	virtual void OnEvent();
	virtual void OffEvent();

	void Reset();

	//ƒtƒ‰ƒOŠÖŒW
	void SetSelected(const bool selected);
	void SetInitExecuteEvent(const bool initExecute);
	void SetRecieveInputEnable(const bool inputRecieve);
	bool IsPressed() const;
	bool IsSelected() const;

	//Ý’è
	void SetEnterButton(const XI::VGP enterButton);
	void SetEnterButton(const XI::Mouse::MB enterButton);
	void SetText(const string& text);
	void Drawtext(const DG::Font::SP& font, const bool drawState);

	void UpDate() override;
};