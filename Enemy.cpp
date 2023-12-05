#include "Enemy.h"

Enemy::Enemy()
	:NPC()
	,range_(0)
{

}

bool Enemy::UpDateState(AIState afterState)
{
	if (nowState_ == afterState)
		return false;
	else
	{
		preState_ = nowState_;
		nowState_ = afterState;
		return true;
	}
}

float Enemy::GetRange() const
{
	return range_;
}

void Enemy::SetRange(const float range)
{
	range_ = range;
}

Enemy::AIState Enemy::GetPreState() const
{
	return preState_;
}

void Enemy::SetPreState(const AIState preState)
{
	preState_ = preState;
}

Enemy::AIState Enemy::GetNowState() const
{
	return nowState_;
}

void Enemy::SetNowState(const AIState nowState)
{
	nowState_ = nowState;
}