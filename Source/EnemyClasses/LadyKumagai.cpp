#include"LadyKumagai.h"
#include "../../Source/Actors/Task_OrionContainer.h"
#include "../../Source/Actors/Enemys/Task_Fish00.h"
#include"../Actors/Task_Player.h"
#include "../Scene/MartialFightScene.h"

LadyKumagai::LadyKumagai()
	:BossLady()
	, standCnt_(60)
	, attackCnt_(90)
{
	AddComponent(containerCD_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	containerCD_->SetCountFrame(120);
	containerCD_->Start();

	AddComponent(fishCD_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	fishCD_->SetCountFrame(180);
	fishCD_->Start();

	status_->HP.Initialize(1000);
	status_->attack.Initialize(15, 100);
	status_->defence.Initialize(0, 100);
	status_->speed.Initialize(2.5f, 100.f, 10.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	gravity_->SetDirection(ML::Vec2::Down());
	gravity_->SetSpeed(0.0f, status_->speed.GetFallSpeed(), 0.5f);
	gravity_->SetAcceleration(ML::Gravity(32) * 10);

	angle_LR_ = Angle_LR::Left;

	SetPreState(AIState::Idle);
	SetNowState(AIState::Idle);

	SetFov(1000.f);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);

	SetTarget(ge->playerPtr.get());

	SetStartPos({ 2530.f,680.f });
}

void LadyKumagai::Think()
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
			afterState = AttackStand;
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
			moveCnt_->SetCountFrame(standCnt_);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(attackCnt_);
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

void LadyKumagai::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();
	ML::Vec2 est;

	//イベント終了後は常にコンテナと魚を召喚
	if (GetNowState()!=AIState::Idle)
	{
		CreateContainer();
		containerCD_->Update();

		SummonFish();
		fishCD_->Update();
	}



	switch (GetNowState())
	{
	case AIState::Idle:
		break;
	case AIState::AttackStand:
		UpDateAttackStand();
		break;
	case AIState::Attack:
		UpDateAttack();
		break;
	case AIState::Damage:
		UpDateDamage();
		break;
	case AIState::Dead:
		UpDateDead();
		break;
	}

	est = GetMoveVec();
	CheckMove(est);

	//ダメージ処理
	if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
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

void LadyKumagai::UpDateAttackStand()
{

}

void LadyKumagai::UpDateAttack()
{
	
}

void LadyKumagai::UpDateDamage()
{
	if (!unHitTimer_->IsCounting())
	{
		status_->HP.TakeDamage(ge->playerPtr->GetStatus()->attack.GetNow());
		unHitTimer_->Start();
	}
}

void LadyKumagai::UpDateDead()
{
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}

void LadyKumagai::CreateContainer()
{
	if (!containerCD_->IsCounting())
	{
		auto con = Container::Object::Create(true);
		con->SetPos(ML::Vec2(GetPos().x + 30, GetPos().y+10));
		containerCD_->Start();
	}
}

void LadyKumagai::SummonFish()
{
	if (!fishCD_->IsCounting())
	{
		auto fish = Fish00::Object::Create(true);
		fish->SetPos(ML::Vec2(GetPos().x + 30, GetPos().y));
		fishCD_->Start();
	}
}

