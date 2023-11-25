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

	float fov_;//����
	float distance_;//�v���C���[�Ǝ��g�̋���

	enum class AIState
	{
		Idle, //�ҋ@
		Patrol, //����
		Approach, //�ڋ�
		Jump, //����
		Attack,//�U��
		Guard,//�h��
		Dodge,//���
		Damage,//�팂
		Dead,//���S
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

