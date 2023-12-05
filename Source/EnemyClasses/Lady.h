#pragma once
#include "Enemy.h"
class Lady : public Enemy
{
public:
	Lady();
	virtual ~Lady() {};

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
};

