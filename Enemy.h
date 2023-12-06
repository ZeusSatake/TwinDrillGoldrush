#pragma once
#include "NPC.h"
#include "myLib.h"
class Enemy : public NPC
{
	float range_;//�˒�
	bool attackFlag_;//�U���t���O
protected:
	shared_ptr<TimerComponent> moveCnt_;
public:
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
	Enemy();
	virtual ~Enemy() {};

	bool WithinRange(class Actor* target);

	bool UpDateState(AIState nowState);

	void BeginAttack();

	bool IsAttacking() const;

	void EndAttack();

	float GetRange() const;
	void SetRange(const float range);

	AIState GetPreState() const;
	void SetPreState(const AIState preState);

	AIState GetNowState() const;
	void SetNowState(const AIState nowState);
};

