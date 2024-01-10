#include "OrionContainer.h"
#include "Source/Actors/Task_Player.h"

OrionContainer::OrionContainer()
	:Actor()
	,movementAmount_(1.5f)
	,limitRange_(100)
{
}

void OrionContainer::Move()
{
	//�d�l��E�����炵�������Ȃ����߁B
	SetMoveVecX(-movementAmount_);

	SetPosX(GetPos().x + GetMoveVec().x);
	
	limitRange_ -= movementAmount_;
	if (ge->playerPtr->CheckHit(this->GetBox()->getHitBase()))
	{
		HitPlayer();
	}
	if (limitRange_ <= 0)
	{
		Kill();
	}
}

void OrionContainer::HitPlayer()
{
	//�v���C���[���̒ǉ��ړ��ʂɎ��g�̈ړ��ʑ��
}
