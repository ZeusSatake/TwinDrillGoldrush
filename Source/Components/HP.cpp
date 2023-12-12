#include "HP.h"
#include <assert.h>

void HP::UpdateIsAlive()
{
	if (now_ > 0) {
		isAlive_ = true;
	}
	else {
		isAlive_ = false;
	}
}

HP::HP(class Actor* owner)
	:Component(owner),
	now_(0),
	max_(0),
	isAlive_(false)
{
}
HP::HP(class Actor* owner,int max_)
	:Component(owner),
	now_(max_),
	max_(max_)
{
	assert(max_ >= 0);
	UpdateIsAlive();
}
void HP::TakeDamage(const int damage_)
{
	now_ -= damage_;
	if (now_ < 0)
		now_ = 0;
	UpdateIsAlive();
}

void HP::TakeHeal(const int heal_)
{
	now_ += heal_;
	if (now_ > max_)
		now_ = max_;
	UpdateIsAlive();
}

int HP::GetNowHP() const
{
	return now_;
}

void HP::SetMaxHP(const int max_, MaxLifeSetMode setMode)
{
	assert(max_ >= 0);

	if (max_ == max_)
		return;

	if (max_ > max_) {
		AddMaxHP(max_ - max_, setMode);
	}
	else {
		SubMaxHP(max_ - max_);
	}
}

void HP::AddMaxHP(const int add, MaxLifeSetMode setMode)
{
	assert(max_ + add > max_);

	max_ += add;

	switch (setMode) {

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

void HP::SubMaxHP(const int sub)
{
	assert(max_ - sub < max_);

	max_ -= sub;
	now_ = max_;

	UpdateIsAlive();
}

int HP::GetMaxHP() const
{
	return max_;
}

void HP::Initialize(const int max_)
{
	if (max_ <= 0) {
		assert(!"‰Šú‰»‚µ‚æ‚¤‚Æ‚µ‚½Å‘åHP‚ª0ˆÈ‰º‚Å‚·");
	}
	
	SetMaxHP(max_, HP::MaxLifeSetMode::MaxHeal);
}

bool HP::IsAlive() const
{
	return isAlive_;
}