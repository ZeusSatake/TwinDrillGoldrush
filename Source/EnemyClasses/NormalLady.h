#pragma once
#include "Lady.h"
class NormalLady : public Lady
{
protected:
	shared_ptr<BoxCollisionComponent> fanEdge_;
public:
	NormalLady();
	virtual ~NormalLady() { };

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
};

