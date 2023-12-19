#include "StatusComponent.h"
#include <assert.h>

void StatusComponent::Speed::Initialize(const float now, const float max, const float fallSpeed)
{
	if (max_ <= 0||fallSpeed<0)
	{
		assert(!"‰Šú‰»‚µ‚æ‚¤‚Æ‚µ‚½’l‚ª0ˆÈ‰º‚Å‚·");
	}
	now_ = now;
	max_ = max;
	fallSpeed_ = fallSpeed;
}

float StatusComponent::Speed::GetNow() const
{
	return now_;
}

float StatusComponent::Speed::GetMax() const
{
	return max_;
}

float StatusComponent::Speed::GetFallSpeed() const
{
	return fallSpeed_;
}

void StatusComponent::Speed::AddSpeed(const float add)
{
	now_ += add;
}

void StatusComponent::Speed::SetMax(const float max)
{
	max_ = max;
}

void StatusComponent::Speed::SetFallSpeed(const float fallSpeed)
{
	fallSpeed_ = fallSpeed;
}

void StatusComponent::Attack::Initialize(const int now,const int max)
{
	if (max_ <= 0)
	{
		assert(!"‰Šú‰»‚µ‚æ‚¤‚Æ‚µ‚½’l‚ª0ˆÈ‰º‚Å‚·");
	}
	now_ = now;
	max_ = max;
}

int StatusComponent::Attack::GetNow() const
{
	return now_;
}

int StatusComponent::Attack::GetMax() const
{
	return max_;
}

void StatusComponent::Attack::AddAttack(const int add)
{
	now_ += add;
}

void StatusComponent::Attack::SetMax(const int max)
{
	max_ = max;
}

void StatusComponent::Defence::Initialize(const int now, const int max)
{
	if (max_ <= 0)
	{
		assert(!"‰Šú‰»‚µ‚æ‚¤‚Æ‚µ‚½’l‚ª0ˆÈ‰º‚Å‚·");
	}
	now_ = now;
	max_ = max;
}

int StatusComponent::Defence::GetNow() const
{
	return now_;
}
int StatusComponent::Defence::GetMax() const
{
	return max_;
}

void StatusComponent::Defence::AddDefence(const int add)
{
	now_ += add;
}

void StatusComponent::Defence::SetMax(const int max)
{
	max_ = max;
}

void StatusComponent::Health::UpdateIsAlive()
{
	if (now_ > 0) 
	{
		isAlive_ = true;
	}
	else 
	{
		isAlive_ = false;
	}
}

void StatusComponent::Health::TakeDamage(const int damage_)
{
	now_ -= damage_;
	if (now_ < 0)
		now_ = 0;
	UpdateIsAlive();
}

void StatusComponent::Health::TakeHeal(const int heal_)
{
	now_ += heal_;
	if (now_ > max_)
		now_ = max_;
	UpdateIsAlive();
}

int StatusComponent::Health::GetNowHP() const
{
	return now_;
}

void StatusComponent::Health::SetMaxHP(const int max_, MaxLifeSetMode setMode)
{
	assert(max_ >= 0);

	if (max_ == max_)
		return;

	if (max_ > max_) 
	{
		AddMaxHP(max_ - max_, setMode);
	}
	else 
	{
		SubMaxHP(max_ - max_);
	}
}

void StatusComponent::Health::AddMaxHP(const int add, MaxLifeSetMode setMode)
{
	assert(max_ + add > max_);

	max_ += add;

	switch (setMode) 
	{
	case MaxLifeSetMode::MaxHeal:
		TakeHeal(this->max_);
		break;

	case MaxLifeSetMode::DifferenceHeal:
		TakeHeal(add);
		break;

	case MaxLifeSetMode::NonHeal:
		break;
	}
}

void StatusComponent::Health::SubMaxHP(const int sub)
{
	assert(max_ - sub < max_);

	max_ -= sub;
	now_ = max_;

	UpdateIsAlive();
}

int StatusComponent::Health::GetMaxHP() const
{
	return max_;
}

void StatusComponent::Health::Initialize(const int max_)
{
	if (max_ <= 0) 
	{
		assert(!"‰Šú‰»‚µ‚æ‚¤‚Æ‚µ‚½Å‘åHP‚ª0ˆÈ‰º‚Å‚·");
	}

	SetMaxHP(max_, Health::MaxLifeSetMode::MaxHeal);
}

bool StatusComponent::Health::IsAlive() const
{
	return isAlive_;
}

