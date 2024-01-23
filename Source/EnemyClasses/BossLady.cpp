#include "BossLady.h"

BossLady::BossLady()
	:Lady()
	, isHitBomb_(false)
{
}

void BossLady::HitBomb()
{
	isHitBomb_ = true;
}

bool BossLady::EndPrison()
{
	return false;
}