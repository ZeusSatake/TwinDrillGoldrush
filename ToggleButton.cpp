#include "ToggleButton.h"

#include "Source/Actors/UI/Task_Cursor.h"

ToggleButton::ToggleButton()
	:
	buttonState_(0),
	text_(""),
	enterButton_(),
	mouseEnterButton_()
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

bool ToggleButton::CanSelectedIsOn()
{
	//マウス
	if (XI::Mouse::SP mouseSP = mouse_.lock())
	{
		const ML::Point& mousePos = mouseSP->GetState().pos;
		if (CheckHit(ML::Box2D(mousePos.x, mousePos.y, 1, 1)))
			return true;
	}

	//選択状態を変更するアクター(カーソル等)
	for (auto& selector : selectors_)
	{
		if (selector->CheckHit(box_->getHitBase().OffsetCopy(pos_)))
			return true;
	}

	return false;
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

void ToggleButton::SetStateText()
{
	stateText_ = "";

	stateText_ += IsPressed() ?
		"Pressed" :
		"NoPressed";

	stateText_ += "\n";

	stateText_ += IsSelected() ?
		"Select" :
		"NoSelect";

	stateText_ += "\n";

	stateText_ += GetButtonState((int)ButtonState::RecieveInput) ?
		"Recieve" :
		"NoRecieve";
}

void ToggleButton::SetRecieveInputEnable(const bool inputRecieve)
{
	SetButtonState((int)ButtonState::RecieveInput, inputRecieve);
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
void ToggleButton::SetEnterButton(const XI::Mouse::MB enterButton)
{
	mouseEnterButton_ = enterButton;
}

void ToggleButton::SetText(const string& text)
{
	text_ = text;
}
void ToggleButton::Drawtext(const DG::Font::SP& font, const bool drawState)
{
	string msg = text_;
	msg += drawState ?
		"\n" + stateText_ :
		"";
	font->Draw(ML::Box2D(pos_.x, pos_.y, 600, 600), msg, ML::Color(1.0f, 1.0f, 0.0f, 0.0f));
}

void ToggleButton::SetMouse(const shared_ptr<XI::Mouse> mouse)
{
	mouse_ = mouse;
}
void ToggleButton::SetSelector(const Actor* selector)
{
	selectors_.push_back(selector);
}

const shared_ptr<const BoxCollisionComponent> ToggleButton::GetBox() const
{
	return box_;
}

void ToggleButton::UpDate()
{
	SetSelected(CanSelectedIsOn());

	SetStateText();

	if (GetButtonState((int)ButtonState::IsSelected | (int)ButtonState::RecieveInput)) 
	{
		if (ge->in1->CheckBT(enterButton_) == XI::GamePad::ButtonState::Down ||
			ge->mouse->CheckBT(mouseEnterButton_) == XI::Mouse::ButtonState::Down)
		{
			ToggleEvent();
		}
	}
}