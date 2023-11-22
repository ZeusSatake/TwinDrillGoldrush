#pragma once
#include "../../Character.h"
#include "../../Component.h"
#include "AIMoveComponent.h"
class AIComponent : public Component
{
public:
	class shared_ptr<AIMoveComponent> move_;
	AIComponent(class Character* owner);
	virtual ~AIComponent() {};

	float fov_;//視野
	float distance_;//プレイヤーと自身の距離

	enum class AIState
	{
		Idle, //待機
		Patrol, //巡回
		Approach, //接近
		Jump, //跳躍
		Attack,//攻撃
		Guard,//防御
		Dodge,//回避
		Damage,//被撃
		Dead,//死亡
	};
	AIState preState_;
	AIState nowState_;
public:
	virtual void Update() override;
	virtual void UpdatePatrol();
	virtual void UpdateApproach();
	virtual void UpdateJump();
	virtual void UpdateAttack();
	virtual void UpdateGuard();
	virtual void UpdateDodge();
	virtual void UpdateDamage();
	virtual void UpdateDead();

	virtual void Think();
	virtual void Move();
protected:
	Actor* target_;
public:
	Actor* GetTarget();
	void SetTarget(Actor* target);

};

