#pragma once
#include "../../Enemy.h"
class Lady : public Enemy
{
public:
	Lady();
	virtual ~Lady() {};

public:
	virtual void Think() {};
	virtual void Move() {};
	virtual void UpDatePatrol() {};
	virtual void UpDateApproach() {};
	virtual void UpDateJump() {};
	virtual void UpDateFall() {};
	virtual void UpDateAttackStand() {};
	virtual void UpDateAttack() {};
	virtual void UpDateGuard() {};
	virtual void UpDateDodge() {};
	virtual void UpDateDamage() {};
	virtual void UpDateDead() {};
};

