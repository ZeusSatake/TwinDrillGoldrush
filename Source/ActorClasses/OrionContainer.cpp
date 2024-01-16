#include "OrionContainer.h"
#include "../Actors/Task_Player.h"

OrionContainer::OrionContainer()
	:Actor()
	,movementAmount_(1.5f)
	,limitRange_(750)
{
	AddComponent(fowardCollision_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
	fowardCollision_->setHitBase(ML::Box2D{ 0,0,1,16 });

	AddComponent(topCollision_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
	topCollision_->setHitBase(ML::Box2D{ 0,0,64,1 });
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
