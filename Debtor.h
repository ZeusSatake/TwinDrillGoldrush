#pragma once
#include "Enemy.h"

//-----------------------------------------------------
//���Ҋ��N���X
//-----------------------------------------------------
class Debtor : public Enemy
{
public:
	Debtor();
	virtual ~Debtor() {};

	enum AIState
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

	virtual void Think();
	virtual void Move();
	virtual void UpDatePatrol();
	virtual void UpDateApproach();
	virtual void UpDateJump();
	virtual void UpDateAttack();
	virtual void UpDateGuard();
	virtual void UpDateDodge();
	virtual void UpDateDamage();
	virtual void UpDateDead();

	bool UpDateState(AIState nowState);

	bool HitPlayer();
};

