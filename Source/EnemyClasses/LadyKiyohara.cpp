#include "LadyKiyohara.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Bomb.h"
#include "../Scene/MartialFightScene.h"

LadyKiyohara::LadyKiyohara()
	:BossLady()
	, defaultFlyPosY_(500.f)
	, attackPattern_(AttackPattern::Non)
	, toGlidingPos_(0.f, 0.f)
	, toVec_(0.f, 0.f)
	, bombDistance_(50.f)
	, patternSwitchFlag_(false)
	, tackleCnt_(5)
{
	SetFov(1000.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	GetStatus()->HP.Initialize(250);
	GetStatus()->speed.Initialize(4.5f, 7.f, 5.f);
	GetStatus()->attack.Initialize(100, 100);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);

	SetTarget(ge->playerPtr.get());
	SetStartPos({ ge->playerPtr->GetPos().x, defaultFlyPosY_ });
}

void LadyKiyohara::Think()
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
			case AttackPattern::DropBombs:
				afterState = AIState::Fly;
				break;
			case AttackPattern::GlidingAttack:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::TackleAttack:
				if (tackleCnt_ > 0)
				{
					afterState = AIState::AttackStand;
				}
				else
				{
					afterState = AIState::Fly;
				}
				break;
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
			afterState = GetPreState();
		}
		break;
	case AIState::Fly:
		if (GetPos().y < defaultFlyPosY_ + 50.f && GetPos().y >= defaultFlyPosY_ - 50.f)
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
			if (attackPattern_ == TackleAttack)
			{
				moveCnt_->SetCountFrame(60);
			}
			else
			{
				moveCnt_->SetCountFrame(120);
			}
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
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void LadyKiyohara::Move()
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
		UpDateDamage();
		break;
	case AIState::Fly:
		UpDateFly();
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

	if (GetNowState()!=AIState::Dead)
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
			attackPattern_ = AttackPattern::DropBombs;
			break;
		case AttackPattern::DropBombs:
			attackPattern_ = AttackPattern::GlidingAttack;
			break;
		case AttackPattern::GlidingAttack:
			UpDateTackleCount();
			attackPattern_ = AttackPattern::TackleAttack;
			break;
		case AttackPattern::TackleAttack:
			attackPattern_ = AttackPattern::DropBombs;
			break;
		}
		patternSwitchFlag_ = false;
	}
}

void LadyKiyohara::UpDateFly()
{
	if (defaultFlyPosY_ < GetPos().y)
	{
		SetMoveVecY(-GetStatus()->speed.GetFallSpeed());
	}
	else
	{
		SetMoveVecY(GetStatus()->speed.GetFallSpeed());
	}
}

void LadyKiyohara::UpDateDamage()
{

}

void LadyKiyohara::UpDateDead()
{
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}

void LadyKiyohara::UpDateAttackStand()
{
	BeginAttack();
	switch (attackPattern_)
	{
	case AttackPattern::Non:
		break;
	case AttackPattern::DropBombs:
		/*if (ge->playerPtr->GetPos().x < GetPos().x)
		{
			SetMoveVecX(-GetStatus()->speed.GetNow());
		}
		else if(ge->playerPtr->GetPos().x>GetPos().x)
		{
			SetMoveVecX(GetStatus()->speed.GetNow());
		}
		else
		{
			SetMoveVecX(0);
		}*/
		SetPosX(ge->playerPtr->GetPos().x);
		SetMoveVecY(0);

		if (isHitBomb_)
		{
			patternSwitchFlag_ = true;
			isHitBomb_ = false;
		}
		break;
	case AttackPattern::GlidingAttack:
		//降下する方向を決定
		SetMoveVec(ML::Vec2{0, 0});
		toVec_ = CalcAngle({ ge->playerPtr->GetPos().x, ge->playerPtr->GetPos().y});
		toGlidingPos_ = ge->playerPtr->GetPos();
		break;
	case AttackPattern::TackleAttack:
		SetMoveVec(ML::Vec2{ 0,0 });
		if (ge->playerPtr->GetPos().x < GetPos().x)
		{
			angle_LR_ = Angle_LR::Left;
		}
		else
		{
			angle_LR_ = Angle_LR::Right;
		}
		break;
	}
}

void LadyKiyohara::UpDateAttack()
{
	switch (attackPattern_)
	{
	case AttackPattern::Non:
		break;
	case AttackPattern::DropBombs:
		UpDateDropBombs();
		break;
	case AttackPattern::GlidingAttack:
		UpDateGlidingAttack();
		break;
	case AttackPattern::TackleAttack:
		UpDateTackleAttack();
		break;
	}
}

void LadyKiyohara::UpDateDropBombs()
{
	
	SetMoveVec({ 0,0 });
	auto bomb00 = Bomb0::Object::Create(true);
	bomb00->SetPos({ GetPos().x,GetPos().y });
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

void LadyKiyohara::UpDateGlidingAttack()
{
	//決定時のY座標まで降下したら攻撃終了
	SetMoveVec(toVec_ * status_->speed.GetFallSpeed());
	//プレイヤーと自身の高さの整合性を取る
	if (toGlidingPos_.y <= GetPos().y+16)
	{
		EndAttack();
		patternSwitchFlag_ = true;
	}
}

void LadyKiyohara::UpDateTackleAttack()
{
	if (tackleCnt_ <= 0)
	{
		patternSwitchFlag_ = true;
	}
	else
	{
		if (angle_LR_==Angle_LR::Left)
		{
			SetMoveVecX(-GetStatus()->speed.GetNow());
		}
		else
		{
			SetMoveVecX(GetStatus()->speed.GetNow());
		}
		if(!moveCnt_->IsCounting())
		{
			--tackleCnt_;
			EndAttack();
		}
	}
}

void LadyKiyohara::UpDateTackleCount()
{
	if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 2)
	{
		GetStatus()->speed.SetNow(GetStatus()->speed.GetNow() * 1.2f);
		tackleCnt_ = 5;
	}
	else if (GetStatus()->HP.GetNowHP() <= GetStatus()->HP.GetMaxHP() / 4)
	{
		GetStatus()->speed.SetNow(GetStatus()->speed.GetNow() * 1.5f);
		tackleCnt_ = 3;
	}
	else
	{
		tackleCnt_ = 7;
	}
}


