#include "LadyHaraguchi.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Tower.h"
#include "../Scene/MartialFightScene.h"
#include "../Actors/Enemys/Task_OfficeWorker.h"

LadyHaraguchi::LadyHaraguchi()
	:BossLady()
	, isCreatedTower_(false)
{
	SetFov(1000.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	GetStatus()->HP.Initialize(200);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);
	SetTarget(ge->playerPtr.get());
	SetStartPos({ 2530.f,680.f });
	ge->playerPtr->SetPosX(500);
	this->render2D_Priority[1] = 0.2f;
}

void LadyHaraguchi::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
	{
		auto mfs = ge->GetTask<MartialFightScene::Object>(MartialFightScene::defGroupName, MartialFightScene::defName);
		if (mfs->EndOfSpawnBossEvent())//イベント終了してから切り替え
		{
			SetPos(GetStartPos());
			SpawnOfficeWorker();
			afterState = AIState::AttackStand;
		}
	}
		break;
	case AIState::AttackStand:
		break;
	case AIState::Attack:
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
	if (GetStatus()->HP.GetNowHP() <= 0)
	{
		afterState = AIState::Dead;
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
		case AIState::Dead:
			moveCnt_->SetCountFrame(90);
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

	//ダメージ処理
	if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
	{
		ML::Box2D plBox = ge->playerPtr->drill_->GetBox()->getHitBase();
		plBox.Offset(ge->playerPtr->drill_->GetAttackPos());
		if (box_->CheckHit(plBox))
		{
			GetStatus()->HP.TakeDamage(ge->playerPtr->GetStatus()->attack.GetNow());
		}
	}

	if (GetNowState() != AIState::Dead)
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->TakeAttack(GetStatus()->attack.GetNow());
		}
	}

	UpDateHP();
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
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}

void LadyHaraguchi::CreateTower()
{
	auto tw = Tower0::Object::Create(true);
	tw->SetPosX(ge->playerPtr->GetPos().x);
}

void LadyHaraguchi::SpawnOfficeWorker()
{
	for (int i = 0; i < 5; ++i)
	{
		auto ow = OfficeWorker::Object::Create(true);
		ow->SetPosX(1500+i*100);
		ow->SetPosY(600);
	}
}
