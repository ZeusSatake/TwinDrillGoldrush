#pragma once
#include "Enemy.h"

//-----------------------------------------------------
//Â–±ÒŠî’êƒNƒ‰ƒX
//-----------------------------------------------------
class Debtor : public Enemy
{
public:
	Debtor();
	virtual ~Debtor() {};

	enum AIState
	{
		Idle, //‘Ò‹@
		Patrol, //„‰ñ
		Approach, //Ú‹ß
		Jump, //’µ–ô
		Fall,//—‰º
		Attack,//UŒ‚
		Guard,//–hŒä
		Dodge,//‰ñ”ğ
		Damage,//”íŒ‚
		Dead,//€–S
	};
private:
	AIState preState_;
	AIState nowState_;
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

	bool UpDateState(AIState nowState);

	bool HitPlayer();
};

