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
		Fall,//����
		Attack,//�U��
		Guard,//�h��
		Dodge,//���
		Damage,//�팂
		Dead,//���S
	};
private:
	AIState preState_;
	AIState nowState_;
public:
	virtual void Think();
	virtual void Move();
	virtual void UpDatePatrol();
	virtual void UpDateApproach();
	virtual void UpDateJump();
	virtual void UpDateFall();
	virtual void UpDateAttack();
	virtual void UpDateGuard();
	virtual void UpDateDodge();
	virtual void UpDateDamage();
	virtual void UpDateDead();

	bool UpDateState(AIState nowState);

	bool HitPlayer();
};

