#pragma once
#include "../../Character.h"
#include "../../Component.h"
#include "AIMoveComponent.h"
class AIComponent : public Component
{
	Actor* target_;
public:
	class shared_ptr<AIMoveComponent> move_;
	AIComponent(class Character* owner);
	virtual ~AIComponent() {};

	enum class AIState
	{
		Idle, //‘Ò‹@
		Patrol, //„‰ñ
		Approach, //Ú‹ß
		Jump, //’µ–ô
		Attack,//UŒ‚
		Guard,//–hŒä
		Dodge,//‰ñ”ğ
		Dead,//€–S
	};
	AIState state_;
public:
	virtual void Update() override;
	virtual void UpdatePatrol();
	virtual void UpdateApproach();
	virtual void UpdateJump();
	virtual void UpdateAttack();
	virtual void UpdateGuard();
	virtual void UpdateDodge();
	virtual void UpdateDead();

	virtual void Think();
	virtual void Move();

	Actor* GetTarget();
	void SetTarget(Actor* target);

};

