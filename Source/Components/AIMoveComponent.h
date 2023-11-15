#pragma once
#include "Component.h"
#include "myLib.h"
#include "Actor.h"
class AIMoveComponent : public Component
{
	ML::Vec2 moveVec_;
	float moveSpeed_;


	enum class AIState
	{
		Idle,
		Patrol,
		Attack,
		Jump,
		Dead
	};

	AIState state_;

	virtual void UpdatePatrol();

	virtual void UpdateAttack();

	virtual void UpdateJump();

	virtual void UpdateDead();
public:
	AIMoveComponent(class Actor* owner);

	virtual ~AIMoveComponent() {};

	void Update() override;

	void MoveTo(class Actor* target);

	void Patroll(class Actor* target);

	void Jump();

	void KnockBack();
};

