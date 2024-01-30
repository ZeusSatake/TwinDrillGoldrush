#include "HPBarComponent.h"

#include "../../Enemy.h"
#include "../../Player.h"

#include "../../myLib.h"

#include "../Actors/UI/Task_DrawGauge.h"

using namespace std;

HPBarComponent::HPBarComponent(Enemy* owner)
	:
	Component(owner),
	status_(*owner->GetStatus()),//
	bar_(DrawGauge::Object::Create(true))
{
	SetHPPercentage();
}
HPBarComponent::HPBarComponent(Player* owner)
	:
	Component(owner),
	status_(*owner->GetStatus()),//
	bar_(DrawGauge::Object::Create(true))
{
	SetHPPercentage();
}
HPBarComponent::~HPBarComponent()
{
	bar_->Kill();
}

void HPBarComponent::SetHPPercentage()
{
	bar_->Set(status_.HP.GetPercentage());
}

void HPBarComponent::SetImg(const string& path)
{
	bar_->SetImg(path);
}
void HPBarComponent::SetDrawSize(const int width, const int height)
{
	bar_->SetDrawSize(width, height);
}
void HPBarComponent::SetPos(const ML::Vec2& pos)
{
	bar_->SetPos(pos);
}
void HPBarComponent::SetPos(const float x, const float y)
{
	bar_->SetPos(x, y);
}
void HPBarComponent::SetSupportScroll(const bool isSupportScroll)
{
	bar_->SetSupportScroll(isSupportScroll);
}
void HPBarComponent::SetVisible(const bool visible)
{
	bar_->SetVisible(visible);
}
void HPBarComponent::SetBackSrc(const ML::Box2D& backSrc)
{
	bar_->SetBackSrc(backSrc);
}
void HPBarComponent::SetInsideSrc(const ML::Box2D& insideSrc)
{
	bar_->SetInsideSrc(insideSrc);
}

ML::Point HPBarComponent::GetSize() const
{
	return bar_->GetSize();
}

void HPBarComponent::Update()
{
	SetHPPercentage();
}