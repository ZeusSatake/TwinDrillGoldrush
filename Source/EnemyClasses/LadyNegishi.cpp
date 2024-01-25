#include "LadyNegishi.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_SobaPrison.h"
#include "../Actors/Task_SobaLaser.h"
#include "../Actors/Task_Sobanomi.h"
#include "../Actors/Task_SobaWire.h"
#include "../Scene/MartialFightScene.h"

LadyNegishi::LadyNegishi()
	:BossLady()
	, isDestroyedWire_(false)
	, isCreateWire_(false)
	, patternSwitchFlag_(false)
	, attackPattern_(AttackPattern::Non)
	, prisonDistance_(100.f)
	, laserCnt_(10)
	, defaultHeight_(620.f)
	, defaultFlyPosY_(670.f)
{
	AddComponent(rainTimer_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	rainTimer_->SetCountFrame(15);

	SetFov(1000.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	GetStatus()->HP.Initialize(200);
	GetStatus()->speed.Initialize(3.f, 5.f, 5.f);
	GetStatus()->attack.Initialize(100, 100);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);

	SetTarget(ge->playerPtr.get());

	SetNowState(AIState::Idle);
	angle_LR_ = Angle_LR::Left;
	
	SetPersonalName("根岸");
	render2D_Priority[1] = 0.2f;
}

void LadyNegishi::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
	{
		auto mfs = ge->GetTask<MartialFightScene::Object>(MartialFightScene::defGroupName, MartialFightScene::defName);
		if (mfs->EndOfSpawnBossEvent())//イベント終了してから切り替え
		{
			patternSwitchFlag_ = true;
			afterState = AttackStand;
		}
	}
		break;
	case AIState::AttackStand:
		if (!moveCnt_->IsCounting())
		{
			afterState = AIState::Attack;
		}
		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			switch (attackPattern_)
			{
			case AttackPattern::ExpandPrison:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::SobaLaser:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::SobanomiRain:
				if (isDestroyedWire_)
				{
					afterState = AIState::Fall;
				}
				else
				{
					afterState = AIState::AttackStand;
				}
				break;
			}
		}
		break;
	case AIState::Damage:
		break;
	case AIState::Dead:
		break;
	case AIState::Fly:
		if (GetPos().y >= defaultFlyPosY_)
		{
			isCreateWire_ = false;
			afterState = AIState::AttackStand;
		}
		afterState = AIState::AttackStand;
		break;
	case AIState::Fall:
		if (!moveCnt_->IsCounting())
		{
			isDestroyedWire_ = false;
			afterState = AIState::Run;
		}
		break;
	case AIState::Run:
		if (CheckFront())
		{
			afterState = AIState::Fly;
		}
		break;
	default:
		break;
	}
	if (GetStatus()->HP.GetNowHP() <= 0)
	{
		afterState = AIState::Dead;
	}
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::AttackStand:
			moveCnt_->SetCountFrame(60);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(60);
			break;
		case AIState::Damage:
			moveCnt_->SetCountFrame(30);
			break;
		case AIState::Dead:
			moveCnt_->SetCountFrame(120);
			break;
		case AIState::Fall:
			moveCnt_->SetCountFrame(300);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void LadyNegishi::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();
	ML::Vec2 est;
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
		break;
	case AIState::Dead:
		UpDateDead();
		break;
	case AIState::Fly:
		UpDateFly();
		break;
	case AIState::Fall:
		UpDateFall();
		break;
	case AIState::Run:
		UpDateRun();
		break;
	default:
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

	if (patternSwitchFlag_)
	{
		switch (attackPattern_)
		{
		case AttackPattern::Non:
			attackPattern_ = AttackPattern::ExpandPrison;
			break;
		case AttackPattern::ExpandPrison:
			UpDateLaserCount();
			attackPattern_ = AttackPattern::SobaLaser;
			break;
		case AttackPattern::SobaLaser:
			attackPattern_ = AttackPattern::SobanomiRain;
			break;
		case AttackPattern::SobanomiRain:
			attackPattern_ = AttackPattern::ExpandPrison;
			break;
		}
		patternSwitchFlag_ = false;
	}
}

void LadyNegishi::UpDateRun()
{
	if (angle_LR_ == Angle_LR::Right)
	{
		SetMoveVecX(GetStatus()->speed.GetMax());
		SetMoveVecY(0.f);
	}
	else
	{
		SetMoveVecX(-GetStatus()->speed.GetMax());
		SetMoveVecY(0.f);
	}
}

void LadyNegishi::UpDateAttackStand()
{
	BeginAttack();
	switch (attackPattern_)
	{
		case AttackPattern::Non:
			break;
		case AttackPattern::ExpandPrison:
			SetPosY(defaultHeight_);
			break;
		case AttackPattern::SobaLaser:
			SetPosY(defaultHeight_);
			break;
		case AttackPattern::SobanomiRain:
			if (angle_LR_ == Angle_LR::Right)
			{
				if (CheckFront())
				{
					angle_LR_ = Angle_LR::Left;
				}
			}
			else
			{
				if (CheckFront())
				{
					angle_LR_ = Angle_LR::Right;
				}
			}
			SetPosY(defaultFlyPosY_);
			break;
	}
}

void LadyNegishi::UpDateAttack()
{
	switch (attackPattern_)
	{
	case AttackPattern::Non:
		break;
	case AttackPattern::ExpandPrison:
		UpDateExpandPrison();
		break;
	case AttackPattern::SobaLaser:
		UpDateSobaLaser();
		break;
	case AttackPattern::SobanomiRain:
		UpDateSobanomiRain();
		break;
	}
}

void LadyNegishi::UpDateDead()
{
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}

void LadyNegishi::UpDateFly()
{
	SetMoveVecX(0.f);
	SetMoveVecY(-GetStatus()->speed.GetNow());
}

void LadyNegishi::UpDateFall()
{
	SetMoveVecX(0.f);
	SetMoveVecY(GetStatus()->speed.GetFallSpeed());
}

void LadyNegishi::UpDateExpandPrison()
{
	float prisonPosX = 0;
	while(prisonPosX<3000)
	{
		auto prison = SobaPrison0::Object::Create(true);
		prison->SetPos({ prisonPosX, 480});
		prison->SetOwner(this);
		prisonPosX += prisonDistance_;
	}
	patternSwitchFlag_ = true;
	EndAttack();
}

void LadyNegishi::UpDateSobaLaser()
{
	SetPosY(defaultHeight_);
	if (laserCnt_ <= 0)
	{
		patternSwitchFlag_ = true;
	}
	auto la = SobaLaser0::Object::Create(true);

	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		la->SetMovementAmount(5.f);
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		la->SetMovementAmount(7.f);
	}
	else
	{
		la->SetMovementAmount(3.f);
	}

	
	int randDir = rand() % 3;
	int randRange = rand() % 30;
	switch (randDir)
	{
		case 0:
		{
			la->SetDirection(SobaLaser::Direction::Left);
			la->SetPosX(ge->playerPtr->GetPos().x + 250);
			la->SetPosY(ge->playerPtr->GetPos().y-16 + randRange);
		}
		break;
		case 1:
		{
			la->SetDirection(SobaLaser::Direction::Right);
			la->SetPosX(ge->playerPtr->GetPos().x - 250);
			la->SetPosY(ge->playerPtr->GetPos().y-16 + randRange);
		}
		break;
		case 2:
		{
			la->SetDirection(SobaLaser::Direction::Down);
			la->SetPosX(ge->playerPtr->GetPos().x + randRange);
			la->SetPosY(ge->playerPtr->GetPos().y - 250);
		}
		break;
	}

	--laserCnt_;
	EndAttack();
}

void LadyNegishi::UpDateSobanomiRain()
{
	if (!isCreateWire_)
	{
		auto wire = SobaWire0::Object::Create(true);
		wire->SetOwner(this);
		wire->SetPos({ 0.f, 650.f });
		if (angle_LR_ == Angle_LR::Right)
		{
			wire->SetWeakPointPos(ML::Vec2{ (float)(1000 + rand() % 1500), 650.f }, 0);
			wire->SetWeakPointPos(ML::Vec2{ (float)(1000 + rand() % 1500), 650.f }, 1);
			wire->SetWeakPointPos(ML::Vec2{ (float)(1000 + rand() % 1500), 650.f }, 2);
		}
		else
		{
			wire->SetWeakPointPos(ML::Vec2{ (float)(rand() % 1500), 650.f }, 0);
			wire->SetWeakPointPos(ML::Vec2{ (float)(rand() % 1500), 650.f }, 1);
			wire->SetWeakPointPos(ML::Vec2{ (float)(rand() % 1500), 650.f }, 2);
		}
		isCreateWire_ = true;
	}
	if (angle_LR_ == Angle_LR::Right)
	{
		if(CheckFront())
		{
			isCreateWire_ = false;
			patternSwitchFlag_ = true;
			EndAttack();
		}
		SetMoveVecX(GetStatus()->speed.GetNow());
		SetMoveVecY(0.f);
	}
	else
	{
		if (CheckFront())
		{
			isCreateWire_ = false;
			patternSwitchFlag_ = true;
			EndAttack();
		}
		SetMoveVecX(-GetStatus()->speed.GetNow());
		SetMoveVecY(0.f);
	}
	rainTimer_->Update();
	if (!rainTimer_->IsCounting())
	{
		auto mi=Sobanomi0::Object::Create(true);
		mi->SetPos({ GetPos().x , GetPos().y });
		mi->SetOwner(this);
		rainTimer_->Start();
	}

	
}

void LadyNegishi::UpDateLaserCount()
{
	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		laserCnt_ = 15;
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		laserCnt_ = 20;
	}
	else
	{
		laserCnt_ = 10;
	}
}

bool LadyNegishi::EndPrison()
{
	if (attackPattern_ == AttackPattern::SobanomiRain)
	{
		return true;
	}
	return false;
}

bool LadyNegishi::EndWire()
{
	if (attackPattern_ == AttackPattern::ExpandPrison||GetNowState()==AIState::Dead)
	{
		return true;
	}
	return false;
}

void LadyNegishi::DestroyedWire()
{
	isDestroyedWire_ = true;
}

void LadyNegishi::PatternSwitch()
{
	patternSwitchFlag_ = true;
}