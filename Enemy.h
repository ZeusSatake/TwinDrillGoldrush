#pragma once
#include "NPC.h"
#include "myLib.h"
class Enemy : public NPC
{
	float range_;//Ë’ö
public:
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
	Enemy();
	virtual ~Enemy() {};

	bool UpDateState(AIState nowState);

	float GetRange() const;
	void SetRange(const float range);

	AIState GetPreState() const;
	void SetPreState(const AIState preState);

	AIState GetNowState() const;
	void SetNowState(const AIState nowState);
};

