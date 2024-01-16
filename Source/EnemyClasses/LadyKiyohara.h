#pragma once
#include "BossLady.h"
class LadyKiyohara : public BossLady
{
public:
	LadyKiyohara();
	virtual ~LadyKiyohara() {};

	void Think() override;
	void Move() override;

	void UpDateAttackStand() override;
	void UpDateAttack() override;
	void UpDateDamage() override;

	void DropBombs();
	void GlidingAttack();


};

