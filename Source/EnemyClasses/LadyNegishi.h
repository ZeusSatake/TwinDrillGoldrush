#pragma once
#include "BossLady.h"
#include "../Components/TimerComponent.h"
class LadyNegishi : public BossLady
{
	bool isDestroyedWire_;
	bool isCreateWire_;
	bool patternSwitchFlag_;
	const float prisonDistance_;
	int laserCnt_;
	const float defaultHeight_;
	const float defaultFlyPosY_;
public:
	enum AttackPattern
	{
		Non,
		ExpandPrison,
		SobaLaser,
		SobanomiRain,
	};
private:
	AttackPattern attackPattern_;
	shared_ptr<TimerComponent> rainTimer_;
public:
	LadyNegishi();
	virtual ~LadyNegishi() {};

	void Think() override;
	void Move() override;

	void UpDateAttackStand() override;
	void UpDateAttack() override;
	void UpDateDead() override;
	void UpDateFly();
	void UpDateFall()override;
	void UpDateRun();

	void UpDateExpandPrison();
	void UpDateSobaLaser();
	void UpDateSobanomiRain();

	void UpDateLaserCount();

	bool EndPrison() override;
	bool EndWire();

	void DestroyedWire();
	void PatternSwitch();
};

