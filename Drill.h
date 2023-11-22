#pragma once
#include "Actor.h"
class Drill :public Actor
{
	int attackPoint;
	int durability;
	float nowAngle;

public:
	Drill();
	virtual ~Drill() { };

	void InitPos(ML::Vec2 pos);//位置の初期化
	void SetAngle(float angle);

	int GetAttackPoint();
	float GetNowAngle();
	ML::Vec2 GetDrillPoint();
	void UpdateDrill();
};