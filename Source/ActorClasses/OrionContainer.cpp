#include "OrionContainer.h"
#include "../Actors/Task_Player.h"

OrionContainer::OrionContainer()
	:Actor()
	,movementAmount_(1.5f)
	,limitRange_(750)
{
}

void OrionContainer::Move()
{
	//�d�l��E�����炵�������Ȃ����߁B
	SetMoveVecX(-movementAmount_);

	SetPosX(GetPos().x + GetMoveVec().x);
	
	limitRange_ -= movementAmount_;
	if (limitRange_ <= 0)
	{
		Kill();
	}

	HitFoward();
	HitTop();
}

void OrionContainer::HitPlayer()
{
	//�v���C���[���̒ǉ��ړ��ʂɎ��g�̈ړ��ʑ��
	
}

void OrionContainer::HitFoward()
{
	ML::Box2D foward = ML::Box2D
	{
		box_->getHitBase().x,
		box_->getHitBase().y - 1,
		1,
		box_->getHitBase().h
	};
	foward.Offset(GetPos());

	if (ge->playerPtr->CheckHit(foward))
	{
		ge->playerPtr->SetExternalVec(ML::Vec2{ -movementAmount_, GetPos().x });
	}
}

void OrionContainer::HitTop()
{
	ML::Box2D foward = ML::Box2D
	{
		box_->getHitBase().x,
		box_->getHitBase().y - 1,
		box_->getHitBase().w,
		1
	};
	foward.Offset(GetPos());

	if (ge->playerPtr->CheckHit(foward))
	{
		ge->playerPtr->SetExternalVec(ML::Vec2{ -movementAmount_, GetPos().x });
	}
}
