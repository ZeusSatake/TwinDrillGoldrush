#include "LadySatake.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_SobaLaser.h"
#include "../Actors/Enemys/Task_Fish00.h"
#include "../Actors/Task_Tower.h"
#include "../Actors/Task_Bomb.h"
#include "../Actors/Task_OrionContainer.h"
#include "../Actors/Task_SobaPrison.h"
#include "../Actors/Task_LongSword.h"

LadySatake::LadySatake()
	:BossLady()	
	, isExpandPrison_(false)
	, isCreatedTower_(false)
	, isFalling_(false)
	, patternSwitchFlag_(false)
	, tackleCnt_(3)
	, laserCnt_(5)
	, containerCnt_(5)
	, preHP_(0)
	, defaultFlyPosY_(400.f)
	, attackPattern_(AttackPattern::Non)
	, bombDistance_(50.f)
	, prisonDistance_(100.f)
	, containerSpawnPos_(0.f,0.f)
{
	AddComponent(swordEdge_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));

	SetFov(1000.f);

	status_->HP.Initialize(100);
	status_->attack.Initialize(50, 100);
	status_->defence.Initialize(0, 100);
	status_->speed.Initialize(3.5f, 12.f, 5.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });

	angle_LR_ = Angle_LR::Left;

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);
	swordEdge_->setHitBase(ML::Box2D{ -4, -16, 8, 32 });

	SetTarget(ge->playerPtr.get());

	SetPersonalName("佐竹");
	SetNowState(AIState::Idle);
}

void LadySatake::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (WithinSight(GetTarget()))
		{
			//SetPos(Start);
			patternSwitchFlag_ = true;
			afterState = AIState::AttackStand;
		}
		break;
	case AIState::AttackStand:
		if(!moveCnt_->IsCounting())
		{
			afterState = AIState::Attack;
		}
		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			switch (attackPattern_)
			{
			case AttackPattern::FishAndLaser:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::TowerAndBomb:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::ContainerAndPrison:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::SlashAndTackle:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::WeaponRain:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::Falling:
				afterState = AIState::AttackStand;
				break;
			}
		}
		break;
	case AIState::Warp:
		if (!moveCnt_->IsCounting())
		{
			afterState = AIState::AttackStand;
		}
		break;
	case AIState::Fly:
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
		{
			switch (attackPattern_)
			{
			case AttackPattern::TowerAndBomb:
				moveCnt_->SetCountFrame(120);
				break;
			case AttackPattern::SlashAndTackle:
				moveCnt_->SetCountFrame(120);
				break;
			case AttackPattern::WeaponRain:
				moveCnt_->SetCountFrame(10);
				break;
			default:
				moveCnt_->SetCountFrame(60);
				break;
			}
		}
		break;
		case AIState::Attack:
		{
			switch (attackPattern_)
			{
			case AttackPattern::ContainerAndPrison:
				moveCnt_->SetCountFrame(90);
				break;
			case AttackPattern::SlashAndTackle:
				moveCnt_->SetCountFrame(120);
				break;
			case AttackPattern::WeaponRain:
				moveCnt_->SetCountFrame(30);
				break;
			case AttackPattern::Falling:
				moveCnt_->SetCountFrame(150);
				break;
			default:
				moveCnt_->SetCountFrame(60);
				break;
			}
		}
		break;
		case AIState::Fall:
			moveCnt_->SetCountFrame(120);
			break;
		case AIState::Warp:
			moveCnt_->SetCountFrame(15);
			break;
		case AIState::Dead:
			moveCnt_->SetCountFrame(120);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void LadySatake::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();
	if (angle_LR_ == Angle_LR::Left)
	{
		swordPos_.x = GetPos().x - 16.f;
		swordPos_.y = GetPos().y;
	}
	else
	{
		swordPos_.x = GetPos().x + 16.f;
		swordPos_.y = GetPos().y;
	}
	ML::Vec2 est;

	switch (GetNowState())
	{
	case AIState::Idle:
		break;
	case AIState::Patrol:
		UpDatePatrol();
		break;
	case AIState::Approach:
		UpDateApproach();
		break;
	case AIState::AttackStand:
		UpDateAttackStand();
		break;
	case AIState::Attack:
		UpDateAttack();
		break;
	case AIState::Fall:
		UpDateFall();
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

	if (preHP_ != GetStatus()->HP.GetNowHP())
	{
		preHP_ = GetStatus()->HP.GetNowHP();
		unHitTimer_->Start();
	}

	if (isHitBomb_)
	{
		patternSwitchFlag_ = true;
		isHitBomb_ = false;
	}

	if (patternSwitchFlag_)
	{
		switch (attackPattern_)
		{
		case AttackPattern::Non:
			attackPattern_ = AttackPattern::FishAndLaser;
			break;
		case AttackPattern::FishAndLaser:
			UpDateLaserCount();
			attackPattern_ = AttackPattern::TowerAndBomb;
			break;
		case AttackPattern::TowerAndBomb:
			if (tower_)
			{
				isCreatedTower_ = false;
				tower_->Kill();
				tower_ = nullptr;
			}
			UpDateContainerCount();
			attackPattern_ = AttackPattern::ContainerAndPrison;
			break;
		case AttackPattern::ContainerAndPrison:
			if (isExpandPrison_)
			{
				isExpandPrison_ = false;
			}
			attackPattern_ = AttackPattern::SlashAndTackle;
			break;
		case AttackPattern::SlashAndTackle:
			UpDateTackleCount();
			attackPattern_ = AttackPattern::WeaponRain;
			break;
		case AttackPattern::WeaponRain:
			rainCount_ = 0;
			attackPattern_ = AttackPattern::Falling;
			break;
		case AttackPattern::Falling:
			attackPattern_ = AttackPattern::FishAndLaser;
		}
		if (isWarped_)
		{
			isWarped_ = false;
		}
		patternSwitchFlag_ = false;
	}
}

void LadySatake::UpDateFall()
{
	if (!CheckFoot())
	{
		SetMoveVecY(GetStatus()->speed.GetFallSpeed());
	}
	else if (!moveCnt_->IsCounting())
	{
		EndAttack();
		patternSwitchFlag_ = true;
	}
}

void LadySatake::UpDateWarp()
{
	switch (attackPattern_)
	{
		case AttackPattern::ContainerAndPrison:
			SetPos({ ge->playerPtr->GetPos().x-160,600.f  });
			break;
	}
}

void LadySatake::UpDateAttackStand()
{
	BeginAttack();
	switch (attackPattern_)
	{
	case AttackPattern::FishAndLaser:
		SetPos({ ge->playerPtr->GetPos().x, defaultFlyPosY_});
			break;
		case AttackPattern::TowerAndBomb:
		SetPos({ ge->playerPtr->GetPos().x, defaultFlyPosY_ });
		containerSpawnPos_.x = ge->playerPtr->GetPos().x + 150;
		containerSpawnPos_.y = 700;
		break;
		case AttackPattern::ContainerAndPrison:
			break;
		case AttackPattern::SlashAndTackle:
			SetMoveVec(ML::Vec2{ 0, 0 });
			if (ge->playerPtr->GetPos().x < GetPos().x)
			{
				angle_LR_ = Angle_LR::Left;
			}
			else
			{
				angle_LR_ = Angle_LR::Right;
			}
			break;
		case AttackPattern::WeaponRain:
			break;
	}
}

void LadySatake::UpDateAttack()
{
	switch (attackPattern_)
	{
		case AttackPattern::FishAndLaser:
			UpDateFishAndLaser();
			break;
		case AttackPattern::TowerAndBomb:
			UpDateTowerAndBomb();
			break;
		case AttackPattern::ContainerAndPrison:
			UpDateContainerAndPrison();
			break;
		case AttackPattern::SlashAndTackle:
			UpDateSlashAndTackle();
			break;
		case AttackPattern::WeaponRain:
			UpDateWeaponRain();
			break;
		case AttackPattern::Falling:
			UpDateFall();
			break;
	}
}

void LadySatake::UpDateDead()
{
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}

void LadySatake::UpDateFishAndLaser()
{
	SetMoveVec(ML::Vec2{ 0, 0 });
	if (!isWarped_)
	{
		SetPosY(defaultFlyPosY_);
	}
	SetPos({ ge->playerPtr->GetPos().x, defaultFlyPosY_ });
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
		la->SetPosY(ge->playerPtr->GetPos().y - 16 + randRange);
	}
	break;
	case 1:
	{
		la->SetDirection(SobaLaser::Direction::Right);
		la->SetPosX(ge->playerPtr->GetPos().x - 250);
		la->SetPosY(ge->playerPtr->GetPos().y - 16 + randRange);
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

	auto fish=Fish00::Object::Create(true);
	fish->SetPos({ ge->playerPtr->GetPos().x + 160.f,ge->playerPtr->GetPos().y });

	--laserCnt_;
	EndAttack();
}

void LadySatake::UpDateTowerAndBomb()
{
	SetMoveVec(ML::Vec2{ 0, 0 });
	if (!isCreatedTower_)
	{
		auto tower = Tower0::Object::Create(true);
		tower_ = tower.get();
		tower->SetPosX(ge->playerPtr->GetPos().x);

		isCreatedTower_ = true;
	}

	auto bomb00 = Bomb0::Object::Create(true);
	bomb00->SetPos({ GetPos().x,GetPos().y});
	bomb00->SetOwner(this);
	auto bomb01 = Bomb0::Object::Create(true);
	bomb01->SetPos({ GetPos().x - bombDistance_ * 2,GetPos().y });
	bomb01->SetOwner(this);
	auto bomb02 = Bomb0::Object::Create(true);
	bomb02->SetPos({ GetPos().x - bombDistance_ * 1,GetPos().y });
	bomb02->SetOwner(this);
	auto bomb03 = Bomb0::Object::Create(true);
	bomb03->SetPos({ GetPos().x + bombDistance_ * 1,GetPos().y });
	bomb03->SetOwner(this);
	auto bomb04 = Bomb0::Object::Create(true);
	bomb04->SetPos({ GetPos().x + bombDistance_ * 2,GetPos().y });
	bomb04->SetOwner(this);

	EndAttack();
}

void LadySatake::UpDateContainerAndPrison()
{
	if (!isWarped_)
	{
		SetPos({ ge->playerPtr->GetPos().x, 660.f});
		isWarped_ = true;
	}
	if (moveCnt_->GetCount() < 30)
	{
		angle_LR_ = Angle_LR::Right;
		SetMoveVecX(GetStatus()->speed.GetNow());
	}
	else
	{
		angle_LR_ = Angle_LR::Left;
		SetMoveVecX(-GetStatus()->speed.GetNow());
	}
	SetMoveVecY(0.f);
	if (containerCnt_ <= 0)
	{
		patternSwitchFlag_ = true;
	}
	if (!isExpandPrison_)
	{
		float prisonPosX = 0;
		while (prisonPosX < 3000)
		{
			auto prison = SobaPrison0::Object::Create(true);
			prison->SetPos({ prisonPosX,480 });
			prison->SetOwner(this);
			prisonPosX += prisonDistance_;
		}
		isExpandPrison_ = true;
	}
	if (!moveCnt_->IsCounting())
	{
		auto container = Container::Object::Create(true);
		container->SetPos(containerSpawnPos_);
		container->SetLimitRange(500.f);
		--containerCnt_;
		EndAttack();
	}
	
}

void LadySatake::UpDateSlashAndTackle()
{
	if (tackleCnt_ <= 0)
	{
		patternSwitchFlag_ = true;
	}
	else
	{
		if (CheckFoot())
		{
			if (angle_LR_ == Angle_LR::Left)
			{
				SetMoveVecX(-GetStatus()->speed.GetMax());
			}
			else
			{
				SetMoveVecX(GetStatus()->speed.GetMax());
			}
			ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
			plBox.Offset(GetTarget()->GetPos());
			ML::Box2D swordBox = swordEdge_->getHitBase();
			swordBox.Offset(swordPos_);
			if (swordBox.Hit(plBox))
			{
				ge->playerPtr->TakeAttack(GetStatus()->attack.GetNow());
				EndAttack();
			}
			else if (!moveCnt_->IsCounting())
			{
				--tackleCnt_;
				EndAttack();
			}
		}
		else
		{
			SetMoveVecX(0.f);
			SetMoveVecY(GetStatus()->speed.GetFallSpeed());
		}
	}
}

void LadySatake::UpDateWeaponRain()
{
	SetMoveVec({ 0.f,0.f });
	if (!isWarped_)
	{
		SetPosY(defaultFlyPosY_);
	}
	SetPosX(ge->playerPtr->GetPos().x);

	if (rainCount_%10==9)
	{
		auto bomb00 = Bomb0::Object::Create(true);
		bomb00->SetPosX(GetPos().x);
		bomb00->SetPosY(ge->playerPtr->GetPos().y - 250);
		bomb00->SetOwner(this);
		++rainCount_;
		EndAttack();
	}
	else
	{
		auto fo = LongSword::Object::Create(true);

		int randRange = rand() % 360;
		fo->SetPosX(GetPos().x - 180 + randRange);
		fo->SetPosY(ge->playerPtr->GetPos().y - 250);
		++rainCount_;
		EndAttack();
	}
}

void LadySatake::UpDateLaserCount()
{
	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		laserCnt_ = 10;
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		laserCnt_ = 15;
	}
	else
	{
		laserCnt_ = 5;
	}
}

void LadySatake::UpDateTackleCount()
{
	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		tackleCnt_ = 5;
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		tackleCnt_ = 7;
	}
	else
	{
		tackleCnt_ = 3;
	}
}

void LadySatake::UpDateContainerCount()
{
	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		containerCnt_ = 7;
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		containerCnt_ = 10;
	}
	else
	{
		containerCnt_ = 5;
	}
}

bool LadySatake::EndPrison()
{
	return !isExpandPrison_;
}