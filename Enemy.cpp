#include "Enemy.h"

Enemy::Enemy()
	:NPC()
	,range_(0)
	,preState_(Idle)
	,nowState_(Idle)
	,attackPattern_(0)
{
	AddComponent(moveCnt_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	AddComponent(status_ = shared_ptr<StatusComponent>(new StatusComponent(this)));
}

bool Enemy::WithinRange(class Actor* target)
{
	//“ñ“_ŠÔ‚Ì’·‚³‚ð’²‚×‚é
	SetDistance(abs(ML::Vec2(GetPos() - target->GetPos()).Length()));
	return abs(GetRange()) > GetDistance();
}

bool Enemy::UpDateState(AIState afterState)
{
	if (nowState_ == afterState) return false;
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

int Enemy::GetAttackPattern() const
{
	return attackPattern_;
}

void Enemy::SetAttackPattern(const int attackPattern)
{
	attackPattern_ = attackPattern;
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

StatusComponent* Enemy::GetStatus() const
{
	return status_.get();
}