#pragma once
#include "Lady.h"

class BossLady : public Lady
{
protected:
	bool isHitBomb_;
public:
	BossLady();
	virtual ~BossLady() {};
	void HitBomb();

	virtual bool EndPrison();
};