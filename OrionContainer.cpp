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
	//仕様上右側からしか撃たないため。
	SetMoveVecX(-movementAmount_);
	limitRange_ -= movementAmount_;
	if (ge->playerPtr->CheckHit(this->GetBox()->getHitBase()))
	{
		HitPlayer();
	}
}

void OrionContainer::HitPlayer()
{
	//プレイヤー側の追加移動量に自身の移動量代入
}
