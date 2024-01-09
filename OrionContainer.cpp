#include "OrionContainer.h"
#include "Source/Actors/Task_Player.h"

OrionContainer::OrionContainer(const float movementAmount)
	:Actor()
	,movementAmount_(movementAmount)
	,limitRange_(100)
{
}

void OrionContainer::Move()
{
	//�d�l��E�����炵�������Ȃ����߁B
	SetMoveVecX(-movementAmount_);
	limitRange_ -= movementAmount_;
	if (ge->playerPtr->CheckHit(this->GetBox()->getHitBase()))
	{
		HitPlayer();
	}
}

void OrionContainer::HitPlayer()
{
	//�v���C���[���̒ǉ��ړ��ʂɎ��g�̈ړ��ʑ��
}
