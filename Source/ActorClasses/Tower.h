#pragma once
#include "../../Actor.h"
class Tower : public Actor
{
	const float startPosY_;
	const float movementAmount_;
	const int noticeCnt_;
	const int decisionCnt_;
	const int stopCnt_;
	const int damage_;
	bool isfellChandelier_;
	enum TowerState
	{
		Idle,//�ҋ@
		Notice, //�\��
		Decision,//����
		Rise,//�㏸
		Stop,//��~
		Down,//���~
	};
	TowerState nowState_;
public:
	Tower();
	virtual ~Tower() {};
	void Think();
	void Move();

	void HitPlayer();
	bool UpDateState(TowerState afterState);

	void UpDateNotice();
	void UpDateDecision();
	void UpDateRise();
	void UpDateStop();
	void UpDateDown();

protected:
	shared_ptr<class TimerComponent> moveCount_;
};


