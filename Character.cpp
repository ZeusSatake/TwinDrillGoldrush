#include "Character.h"
#include "Source/Scene/Task_Map.h"

Character::Character()
	:Actor()
{
	//コンポーネントを追加

	AddComponent(hp_ = shared_ptr<HP>(new HP(this)));
	AddComponent(movement_ = shared_ptr<Movement>(new Movement(this)));
	AddComponent(gravity_ = shared_ptr<Movement>(new Movement(this)));
	AddComponent(animManager_ = shared_ptr<AnimManager>(new AnimManager(this,0)));
	AddComponent(unHitTimer_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
}

void Character::Think()
{

}

void Character::Move()
{

}

bool Character::CheckFoot() const
{
	//当たり判定を基に足元の矩形を作成
	ML::Box2D foot = ML::Box2D
	{
		box_->getHitBase().x,
		box_->getHitBase().y + box_->getHitBase().h,
		box_->getHitBase().w,
		1
	};
	foot.Offset(GetPos());

	auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (map == nullptr)
	{
		return false;
	}

	return map->CheckHit(foot);
}

shared_ptr<HP> Character::GetHP() const
{
	return hp_;
}

std::string Character::GetPersonalName() const
{
	return personalName_;
}

void Character::SetPersonalName(const std::string name)
{
	personalName_ = name;
}