#pragma once
#include "Lady.h"
class RegentLady : public Lady
{
	int coolTime_;
	public:
	RegentLady();
	virtual ~RegentLady() {};

	void Think()override;
	void Move()override;
	void UpDateApproach()override;
	void UpDateAttackStand()override;
	void UpDateAttack()override;
	void UpDateDead()override;

};

