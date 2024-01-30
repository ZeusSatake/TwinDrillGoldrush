#pragma once
#include "BossLady.h"
class LadyHaraguchi : public BossLady
{
	bool isCreatedTower_;
public:
	LadyHaraguchi();
	virtual ~LadyHaraguchi() {};

	void Think() override;
	void Move() override;

	void UpDateDamage() override;
	void UpDateDead() override;

	void CreateTower();
	void SpawnOfficeWorker();
};

