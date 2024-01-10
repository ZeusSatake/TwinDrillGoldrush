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
	//仕様上右側からしか撃たないため。
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
	//プレイヤー側の追加移動量に自身の移動量代入
}
