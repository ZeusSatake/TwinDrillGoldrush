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
		Idle, //�ҋ@
		Patrol, //����
		Approach, //�ڋ�
		Jump, //����
		Attack,//�U��
		Guard,//�h��
		Dodge,//���
		Dead,//���S
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

