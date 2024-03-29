#pragma once
#include "NPC.h"
#include "myLib.h"
#include "Source/Components/StatusComponent.h"

class HPBarComponent;

class Enemy : public NPC
{
	float range_;//Ëö
	bool attackFlag_;//UtO
	int attackPattern_;//Up^[
	int preHP_;//Ot[ĚHP
protected:
	shared_ptr<TimerComponent> moveCnt_;
	shared_ptr<StatusComponent> status_;
	shared_ptr<HPBarComponent> hpBar_;
public:
	enum AIState
	{
		Idle, //Ň@
		Stand, //§ż
		Walk, //ŕs
		Run, //s
		Patrol, //ń
		Approach, //Úß
		Jump, //ľô
		Fall,//ş
		Fly,//Řó
		Warp,//[v
		AttackStand,//Uő
		Attack,//U
		Guard,//hä
		Dodge,//ńđ
		Damage,//í
		Dead,//S
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

	void UpDateHP();
	void SetPreHP(int preHP) { preHP_ = preHP; }
};

