#include "HP.h"
#include <assert.h>

void HP::UpdateIsAlive()
{
	if (now > 0) {
		isAlive = true;
	}
	else {
		isAlive = false;
	}
}

HP::HP(class Actor* owner)
	:Component(owner),
	now(0),
	max(0),
	isAlive(false)
{
}
HP::HP(class Actor* owner,int max_)
	:Component(owner),
	now(max_),
	max(max_)
{
	assert(max >= 0);
	UpdateIsAlive();
}
void HP::TakeDamage(const int damage_)
{
	now -= damage_;
	if (now < 0)
		now = 0;
	UpdateIsAlive();
}

void HP::TakeHeal(const int heal_)
{
	now += heal_;
	if (now > max)
		now = max;
	UpdateIsAlive();
}

int HP::GetNowHP() const
{
	return now;
}

void HP::SetMaxHP(const int max_, MaxLifeSetMode setMode)
{
	assert(max_ >= 0);

	if (max_ == max)
		return;

	if (max_ > max) {
		AddMaxHP(max_ - max, setMode);
	}
	else {
		SubMaxHP(max - max_);
	}
}

void HP::AddMaxHP(const int add, MaxLifeSetMode setMode)
{
	assert(max + add > max);

	max += add;

	switch (setMode) {

	case MaxLifeSetMode::MaxHeal:
		TakeHeal(this->max);
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
	assert(max - sub < max);

	max -= sub;
	now = max;

	UpdateIsAlive();
}

int HP::GetMaxHP() const
{
	return max;
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
	return isAlive;
}