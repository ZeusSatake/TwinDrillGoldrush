#include "BossLady.h"

BossLady::BossLady()
	:Lady()
	, isHitBomb_(false)
{
}

void BossLady::UpDateHP()
{
	if (preHP_ != GetStatus()->HP.GetNowHP())
	{
		preHP_ = GetStatus()->HP.GetNowHP();
		unHitTimer_->Start();
	}
}

void BossLady::HitBomb()
{
	isHitBomb_ = true;
}

bool BossLady::EndPrison()
{
	return false;
}