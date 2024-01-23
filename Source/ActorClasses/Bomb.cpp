#include "Bomb.h"
#include "../Actors/Task_Player.h"

Bomb::Bomb()
	:Actor()
	, isReflected_(false)
	, damage_(10)
	, movementAmount_(5.5f)
	, bombState_(BombState::Fall)
{
	AddComponent(bombCnt_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	bombCnt_->SetCountFrame(240);
	bombCnt_->Start();

	box_->setHitBase(ML::Box2D(-8, -8, 16, 16));

	AddComponent(blastBox_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
	blastBox_->setHitBase(ML::Box2D(-16, -16, 32, 32));
}

void Bomb::Move()
{
	bombCnt_->Update();
	ML::Vec2 est;

	switch (bombState_)
	{
	case BombState::Non:
		break;
	case BombState::Fall:
		SetMoveVecY(movementAmount_);
		//ドリル攻撃に当たったら反射
		if (ge->playerPtr->pState == StateComponent::State::Attack)
		{
			ML::Box2D drill = ge->playerPtr->drill_->GetBox()->getHitBase();
			drill.Offset(ge->playerPtr->drill_->GetPos());

			if (box_->CheckHit(drill))
			{
				ChangeReflect();
			}
		}
		break;
	case BombState::Reflect:
	{
		SetMoveVecY(-movementAmount_);

		if (owner_!=nullptr)
		{
			ML::Box2D ownerBox = owner_->GetBox()->getHitBase();
			ownerBox.Offset(owner_->GetPos());
			if (blastBox_->CheckHit(ownerBox))
			{
				bombState_ = Explosion;
			}
		}
	}
		break;
	case BombState::Explosion:
	{
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());
		if (blastBox_->CheckHit(plBox))
		{
			ge->playerPtr->TakeAttack(damage_);
		}
		if (!bombCnt_->IsCounting())
		{
			Kill();
		}
		if (isReflected_&&!isGiveDamage_)
		{
			ML::Box2D ownerBox = owner_->GetBox()->getHitBase();
			ownerBox.Offset(owner_->GetPos());
			if (blastBox_->CheckHit(ownerBox))
			{
				isGiveDamage_ = true;
				owner_->HitBomb();
				owner_->GetStatus()->HP.TakeDamage(damage_);
			}
		}
	}
		break;
	}

	

	if (bombState_ != BombState::Explosion)
	{
		//プレイヤーに当たったら爆発
		ML::Box2D plBox = ge->playerPtr->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->GetPos());
		if (box_->CheckHit(plBox))
		{
			ChangeExplosion();
		}
	}
	est = GetMoveVec();
	CheckMove(est);

	if (!bombCnt_->IsCounting())
	{
		bombState_ = BombState::Explosion;
	}
}

void Bomb::ChangeReflect()
{
	bombState_ = BombState::Reflect;
	isReflected_ = true;
	bombCnt_->Start();
}

void Bomb::ChangeExplosion()
{
	bombState_ = BombState::Explosion;
	bombCnt_->SetCountFrame(30);
	bombCnt_->Start();
}