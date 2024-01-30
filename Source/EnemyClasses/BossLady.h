#pragma once
#include "Lady.h"

class BossLady : public Lady
{
	ML::Vec2 startPos_;

protected:
	bool isHitBomb_;
public:
	BossLady();
	virtual ~BossLady() {};
	void HitBomb();

	virtual bool EndPrison();

	ML::Vec2 GetStartPos() { return startPos_; }
	void SetStartPos(const ML::Vec2 startPos) { startPos_ = startPos; }

};