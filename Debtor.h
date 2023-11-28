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
		Attack,//UŒ‚
		Guard,//–hŒä
		Dodge,//‰ñ”ğ
		Damage,//”íŒ‚
		Dead,//€–S
	};
	AIState preState_;
	AIState nowState_;

	virtual void Think();
	virtual void Move();
	virtual void UpDatePatrol();
	virtual void UpDateApproach();
	virtual void UpDateJump();
	virtual void UpDateAttack();
	virtual void UpDateGuard();
	virtual void UpDateDodge();
	virtual void UpDateDamage();
	virtual void UpDateDead();

	bool UpDateState(AIState nowState);

	bool HitPlayer();
};

