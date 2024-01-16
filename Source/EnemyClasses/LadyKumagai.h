#pragma once
#include "BossLady.h"

class LadyKumagai : public BossLady
{
	const int standCnt_;
	const int attackCnt_;
protected:
	shared_ptr<TimerComponent> containerCD_;
	shared_ptr<TimerComponent> fishCD_;
public:
	LadyKumagai();
	virtual ~LadyKumagai() {};
protected:
	virtual void Think();
	virtual void Move();
	virtual void UpDateAttackStand() override;
	virtual void UpDateAttack() override;
	virtual void UpDateDamage() override;
	virtual void UpDateDead() override;

	void CreateContainer();
	void SummonFish();
};