#include "Enemy.h"

Enemy::Enemy()
	:NPC()
	,range_(0)
{

}

float Enemy::GetRange() const
{
	return range_;
}

void Enemy::SetRange(const float range)
{
	range_ = range;
}