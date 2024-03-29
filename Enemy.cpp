#include "Enemy.h"
#include "Source/Components/HPBarComponent.h"

Enemy::Enemy()
	:NPC()
	,range_(0)
	,preState_(Idle)
	,nowState_(Idle)
	,attackPattern_(0)
{
	AddComponent(moveCnt_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	AddComponent(status_ = shared_ptr<StatusComponent>(new StatusComponent(this)));
	AddComponent(hpBar_ = shared_ptr<HPBarComponent>(new HPBarComponent(this)));

	hpBar_->SetVisible(true);
	hpBar_->SetSupportScroll(true);
	hpBar_->SetDrawSize(75, 15);
	hpBar_->SetImg("./data/image/ui/hpBar/GUISprite.png");
	hpBar_->SetBackSrc(ML::Box2D(80, 48, 64, 16));
	hpBar_->SetInsideSrc(ML::Box2D(80, 32, 64, 16));

	this->render2D_Priority[1] = 0.9f;
}

bool Enemy::WithinRange(class Actor* target)
{
	//二点間の長さを調べる
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

void Enemy::UpDate()
{
	NPC::UpDate();

	//hpバーの位置を足元に同期
	float bodyHeight = GetBox()->getHitBase().h;
	float hpBarHeight = hpBar_->GetSize().y;
	float footDistance = (bodyHeight + hpBarHeight) * 0.5f;
	hpBar_->SetPos(GetPos().x, GetPos().y + footDistance);

	hpBar_->Update();
}

void Enemy::UpDateHP()
{
	if (preHP_ != GetStatus()->HP.GetNowHP())
	{
		preHP_ = GetStatus()->HP.GetNowHP();
		unHitTimer_->Start();
	}
}