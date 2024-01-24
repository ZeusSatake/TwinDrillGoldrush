#include "LadyHaraguchi.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Tower.h"
#include "../../Source/Scene/Task_Map.h"

LadyHaraguchi::LadyHaraguchi()
	:BossLady()
	, isCreatedTerrain_(false)
	, isCreatedTower_(false)
{
	SetFov(1000.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	GetStatus()->HP.Initialize(200);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);
	SetTarget(ge->playerPtr.get());
	SetStartPos({ 2530.f,680.f });
}

void LadyHaraguchi::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (WithinSight(GetTarget()))
		{
			afterState = AIState::AttackStand;
		}
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::AttackStand:
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::Attack:
		if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
		{
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			if (box_->CheckHit(plBox))
			{
				afterState = AIState::Damage;
			}
		}
		break;
	case AIState::Damage:
		if (status_->HP.GetNowHP() <= 0)
		{
			afterState = AIState::Dead;
		}
		else if (!moveCnt_->IsCounting())
		{
			afterState = AIState::AttackStand;
		}
		break;
	case AIState::Dead:

		break;
	}
	//状態の更新と各状態ごとの行動カウンタを設定
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::AttackStand:
			moveCnt_->SetCountFrame(0);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(0);
			break;
		case AIState::Damage:
			moveCnt_->SetCountFrame(30);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void LadyHaraguchi::Move()
{
	//タイマーの更新
	moveCnt_->Update();
	unHitTimer_->Update();

	if (GetNowState() != AIState::Idle)
	{
		if (!isCreatedTower_)
		{
			CreateTower();
			isCreatedTower_ = true;
		}
		if (!isCreatedTerrain_)
		{
			CreateTerrain();
			isCreatedTerrain_ = true;
		}
	}
	//状態に応じた行動
	switch (GetNowState())
	{
		case AIState::Idle:
			break;
		case AIState::AttackStand:
			break;
		case AIState::Attack:
			break;
		case AIState::Damage:
			UpDateDamage();
			break;
		case AIState::Dead:
			UpDateDead();
			break;
	}
}

void LadyHaraguchi::UpDateDamage()
{
	if (!unHitTimer_->IsCounting())
	{
		GetStatus()->HP.TakeDamage(ge->playerPtr->GetStatus()->attack.GetNow());
		unHitTimer_->Start();
	}
}

void LadyHaraguchi::UpDateDead()
{
	Kill();
}

void LadyHaraguchi::CreateTower()
{
	auto tw = Tower0::Object::Create(true);
	tw->SetPosX(ge->playerPtr->GetPos().x);
}

void LadyHaraguchi::CreateTerrain()
{
	auto mapEX = Map::Object::Create(true);
	mapEX->Load("MartialFightEX");
}
