#pragma once
#include "../../Actor.h"

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

	weak_ptr<XI::Mouse> mouse_;
	vector<const Actor*> selectors_;
	ML::Color textColor_;

	void SetButtonState(const int bit, const bool flag);
	bool GetButtonState(const int bit) const;

	void SetStateText();

	void ToggleEvent();

	bool CanSelectedIsOn();
public:
	ToggleButton();
	virtual ~ToggleButton() {};

	virtual void OnEvent();
	virtual void OffEvent();

	void Reset();

	//ÉtÉâÉOån
	void SetSelected(const bool selected);
	void SetInitExecuteEvent(const bool initExecute);
	void SetRecieveInputEnable(const bool inputRecieve);
	bool IsPressed() const;
	bool IsSelected() const;

	//ê›íË
	void SetEnterButton(const XI::VGP enterButton);
	void SetEnterButton(const XI::Mouse::MB enterButton);
	void SetText(const string& text);
	void Drawtext(const DG::Font::SP& font, const bool drawState);
	void SetMouse(const shared_ptr<XI::Mouse> mouse);
	void SetSelector(const Actor* selector);
	void SetTextColor(const ML::Color& color);

	const shared_ptr<const BoxCollisionComponent> GetBox() const;

	void UpDate() override;
};