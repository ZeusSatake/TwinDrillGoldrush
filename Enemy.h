#pragma once
#include "NPC.h"
#include "myLib.h"
#include "Source/Components/StatusComponent.h"

class HPBarComponent;

class Enemy : public NPC
{
	float range_;//Ë’ö
	bool attackFlag_;//UŒ‚ƒtƒ‰ƒO
	int attackPattern_;//UŒ‚ƒpƒ^[ƒ“
protected:
	shared_ptr<TimerComponent> moveCnt_;
	shared_ptr<StatusComponent> status_;
	shared_ptr<HPBarComponent> hpBar_;
public:
	enum AIState
	{
		Idle, //‘Ò‹@
		Stand, //—§‚¿
		Walk, //•às
		Run, //‘–s
		Patrol, //„‰ñ
		Approach, //Ú‹ß
		Jump, //’µ–ô
		Fall,//—‰º
		Fly,//‘Ø‹ó
		AttackStand,//UŒ‚€”õ
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

	bool WithinRange(class Actor* target);

	bool UpDateState(AIState nowState);

	void BeginAttack();

	bool IsAttacking() const;

	void EndAttack();

	float GetRange() const;
	void SetRange(const float range);

	int GetAttackPattern() const;
	void SetAttackPattern(const int attackPattern);

	AIState GetPreState() const;
	void SetPreState(const AIState preState);

	AIState GetNowState() const;
	void SetNowState(const AIState nowState);

	StatusComponent* GetStatus()const;

	void UpDate() override;
};

