#include "NPC.h"
#include "Source/Scene/Task_Map.h"

NPC::NPC()
:Character()
{
	
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
	if (this->angle_LR == Angle_LR::Left)
	{
		front.Offset(-1, 0);//����
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//�E��
	}
	//���݈ʒu�ɍ��킹��
	front.Offset((int)pos_.x, (int)pos_.y);
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
	if (this->angle_LR == Angle_LR::Left)
	{
		front.Offset(-1, 0);//����
	}
	else
	{
		front.Offset(box_->getHitBase().w, 0);//�E��
	}
	//�����ɍ��킹��
	front.Offset((int)pos_.x, (int)pos_.y + box_->getHitBase().h);
	auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return false; }
	return map->CheckHit(front);
}