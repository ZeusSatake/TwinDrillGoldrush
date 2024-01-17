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
	//�L�����N�^�̌����ɂ���`�̈ʒu�𒲐�
	if (this->angle_LR_ == Angle_LR::Left)
	{
		front.Offset(-1, 0);//����
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//�E��
	}
	//���݈ʒu�ɍ��킹��
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
	//�L�����N�^�̌����ɂ���`�̈ʒu�𒲐�
	if (this->angle_LR_ == Angle_LR::Left)
	{
		front.Offset(-1, 0);//����
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//�E��
	}
	//�����ɍ��킹��
	front.Offset((int)GetPos().x, (int)GetPos().y + box_->getHitBase().h);
	auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return false; }
	return map->CheckHit(front);
}

bool NPC::WithinSight(class Actor* target)
{
	//��_�Ԃ̒����𒲂ׂ�
	distance_ = ML::Vec2(GetPos() - target->GetPos()).Length();
	//���Ŏ���p�𖳎����č��E�݂̂ł̔���
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