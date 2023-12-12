#include "Enemy.h"

Enemy::Enemy()
	:NPC()
	,range_(0)
	,preState_(Idle)
	,nowState_(Idle)
{
	AddComponent(moveCnt_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
}

bool Enemy::WithinRange(class Actor* target)
{
	//“ñ“_ŠÔ‚Ì’·‚³‚ð’²‚×‚é
	SetDistance(abs(ML::Vec2(GetPos() - target->GetPos()).Length()));
	return abs(GetRange()) > GetDistance();
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

void Enemy::BeginAttack()
{
	attackFlag_ = true;
}

bool Enemy::IsAttacking() const
{
	return attackFlag_;
}

void Enemy::EndAttack()
{
	attackFlag_ = false;
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