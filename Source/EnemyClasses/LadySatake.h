#pragma once
#include "BossLady.h"
#include "../ActorClasses/Tower.h"

class LadySatake : public BossLady
{
	bool isWarped_;
	bool isExpandPrison_;
	bool isCreatedTower_;
	bool patternSwitchFlag_;
	int tackleCnt_;
	int laserCnt_;
	int containerCnt_;
	int preHP_;	
	const float defaultFlyPosY_;
	const float bombDistance_;
	const float prisonDistance_;
	int rainCount_;
	ML::Vec2 swordPos_;
	Tower* tower_;
	ML::Vec2 containerSpawnPos_;

public:
	enum AttackPattern
	{
		Non,
		FishAndLaser,
		TowerAndBomb,
		ContainerAndPrison,
		SlashAndTackle,
		WeaponRain,
	};
private:
	AttackPattern attackPattern_;
public:
	LadySatake();
	virtual ~LadySatake() {};
protected:
	shared_ptr<BoxCollisionComponent> swordEdge_;
public:
	virtual void Think();
	virtual void Move();
	virtual void UpDateFall() override;
	virtual void UpDateWarp();
	virtual void UpDateAttackStand() override;
	virtual void UpDateAttack() override;
	virtual void UpDateDead() override;

	void UpDateFishAndLaser();
	void UpDateTowerAndBomb();
	void UpDateContainerAndPrison();
	void UpDateSlashAndTackle();
	void UpDateWeaponRain();

	void UpDateTackleCount();
	void UpDateLaserCount();
	void UpDateContainerCount();

	bool EndPrison() override;

	ML::Vec2 GetSwordPos() { return swordPos_; }
};