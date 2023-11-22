#include "Drill.h"
#include "MyPG.h"
Drill::Drill()
	:
	attackPoint(0),
	nowAngle(0.0f),
	durability(0)
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

float Drill::GetNowAngle()
{
	return this->nowAngle;
}

ML::Vec2 Drill::GetDrillPoint()
{
	return this->pos_;
}

void Drill::UpdateDrill()
{
	
}