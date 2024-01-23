#pragma once
#include "NPC.h"
#include "myLib.h"
#include "Source/Components/StatusComponent.h"

class HPBarComponent;

class Enemy : public NPC
{
	float range_;//射程
	bool attackFlag_;//攻撃フラグ
	int attackPattern_;//攻撃パターン
protected:
	shared_ptr<TimerComponent> moveCnt_;
	shared_ptr<StatusComponent> status_;
	shared_ptr<HPBarComponent> hpBar_;
public:
	enum AIState
	{
		Idle, //待機
		Stand, //立ち
		Walk, //歩行
		Run, //走行
		Patrol, //巡回
		Approach, //接近
		Jump, //跳躍
		Fall,//落下
		Fly,//滞空
		Warp,//ワープ
		AttackStand,//攻撃準備
		Attack,//攻撃
		Guard,//防御
		Dodge,//回避
		Damage,//被撃
		Dead,//死亡
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

