#include "Drill.h"
#include "MyPG.h"
Drill::Drill()
{
	
}

void Drill::InitPos(ML::Vec2 pos)
{
	this->pos_ = pos;
}

void Drill::SetAngle(float angle)
{
	this->angle_ = angle;
}

int Drill::GetAttackPoint()
{
	return this->attackPoint;
}

ML::Vec2 Drill::GetDrillPoint()
{
	return this->pos_;
}

void Drill::UpdateDrill()
{
	
}