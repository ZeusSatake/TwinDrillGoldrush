#pragma once
#include "Enemy.h"
class Lady : public Enemy
{
public:
	Lady();
	virtual ~Lady() {};

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
};

