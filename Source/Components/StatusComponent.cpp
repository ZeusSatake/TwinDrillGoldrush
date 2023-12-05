#include "StatusComponent.h"

StatusComponent::StatusComponent(class Actor* owner)
	: Component(owner)
	, hp_(owner,0)
	, attackVal_(0)
	, defenseVal_(0)
	, speedVal_(0.f)
{
}

HP StatusComponent::GetHP() const
{
	return hp_;
}

int StatusComponent::GetAttackVal() const
{
	return attackVal_;
}

void StatusComponent::SetAttackVal(const int attack)
{
	attackVal_ = attack;
}

int StatusComponent::GetDefenseVal() const
{
	return defenseVal_;
}

void StatusComponent::SetDefenseVal(const int defense)
{
	defenseVal_ = defense;
}

float StatusComponent::GetSpeedVal() const
{
	return speedVal_;
}

void StatusComponent::SetSpeedVal(const float speed)
{
	speedVal_ = speed;
}
