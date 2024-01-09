#pragma once
#include "../../Enemy.h"

//-----------------------------------------------------
//ç¬ñ±é“äÓíÍÉNÉâÉX
//-----------------------------------------------------
class Debtor : public Enemy
{
public:
	Debtor();
	virtual ~Debtor() {};

	
public:
	virtual void Think();
	virtual void Move();
	virtual void UpDatePatrol();
	virtual void UpDateApproach();
	virtual void UpDateJump();
	virtual void UpDateFall();
	virtual void UpDateAttack();
	virtual void UpDateGuard();
	virtual void UpDateDodge();
	virtual void UpDateDamage();
	virtual void UpDateDead();

	

	bool HitPlayer();
};

