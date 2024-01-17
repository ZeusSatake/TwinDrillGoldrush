#pragma once
#include "BossLady.h"
class LadyKiyohara : public BossLady
{
	ML::Vec2 toGlidingPos_;
	ML::Vec2 toVec_;
	const float defaultFlyPosY_;
	int preHP_;
	enum AttackPattern
	{
		Non,
		DropBombs,
		GlidingAttack,
		TackleAttack,
	};
	AttackPattern attackPattern_;
	const float bombDistance_;
public:
	LadyKiyohara();
	virtual ~LadyKiyohara() {};

	void Think() override;
	void Move() override;

	void UpDateAttackStand() override;
	void UpDateAttack() override;
	void UpDateDamage() override;
	void UpDateDead() override;
	void UpDateFly();

	void UpDateDropBombs();
	void UpDateGlidingAttack();
	void UpDateTackleAttack();


	//êiçsï˚å¸ÇãÅÇﬂÇÈ
	
};

