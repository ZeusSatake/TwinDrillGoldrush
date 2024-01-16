#include "NPC.h"
#include "Source/Scene/Task_Map.h"

NPC::NPC()
:Character()
{
	AddComponent(AIMove_ = shared_ptr<AIMoveComponent>(new AIMoveComponent(this)));
}

bool NPC::CheckFront() const
{
	ML::Box2D front
	(
		box_->getHitBase().x,
		box_->getHitBase().y,
		1,
		box_->getHitBase().h
	);
	//キャラクタの向きにより矩形の位置を調整
	if (this->angle_LR_ == Angle_LR::Left)
	{
		front.Offset(-1, 0);//左側
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//右側
	}
	//現在位置に合わせる
	front.Offset((int)GetPos().x, (int)GetPos().y);
	auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return false; }
	return map->CheckHit(front);
}

bool NPC::CheckFrontFoot() const
{
	ML::Box2D front
	(
		box_->getHitBase().x+box_->getHitBase().h,
		box_->getHitBase().y,
		1,
		1
	);
	//キャラクタの向きにより矩形の位置を調整
	if (this->angle_LR_ == Angle_LR::Left)
	{
		front.Offset(-1, 0);//左側
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//右側
	}
	//足元に合わせる
	front.Offset((int)GetPos().x, (int)GetPos().y + box_->getHitBase().h);
	auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return false; }
	return map->CheckHit(front);
}

bool NPC::WithinSight(class Actor* target)
{
	//二点間の長さを調べる
	distance_ = ML::Vec2(GetPos() - target->GetPos()).Length();
	//仮で視野角を無視して左右のみでの判定
	if (angle_LR_ == Angle_LR::Left)
	{
		if (GetPos().x > target->GetPos().x)
		{
			return abs(fov_) > distance_;
		}
		return false;
	}
	else
	{
		if (GetPos().x <= target->GetPos().x)
		{
			return abs(fov_) > distance_;
		}
		return false;
	}
}

ML::Vec2 NPC::CalcAngle(ML::Vec2 targetPos)
{
	ML::Vec2 angle;
	angle.x = targetPos.x - GetPos().x;
	angle.y = targetPos.y - GetPos().y;
	angle = angle.Normalize();
	return angle;
}

float NPC::GetFov() const
{
	return fov_;
}

void NPC::SetFov(const float fov)
{
	fov_ = fov;
}

float NPC::GetDistance() const
{
	return distance_;
}

void NPC::SetDistance(const float distance)
{
	distance_ = distance;
}

Actor* NPC::GetTarget() const
{
	return target_;
}

void NPC::SetTarget(Actor* target)
{
	target_ = target;
}