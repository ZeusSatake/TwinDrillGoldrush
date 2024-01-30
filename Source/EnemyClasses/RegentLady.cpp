#include "RegentLady.h"

#include "../Actors/Task_Player.h"
#include "../System/Task_Save.h"
#include "../Actors/Task_Regent.h"

RegentLady::RegentLady()
	:Lady()
	, coolTime_(0)
{
	auto save = ge->GetTask<Save::Object>(Save::defGroupName, Save::defName);
	switch (save->GetValue<int>(Save::Object::ValueKind::StageNo))
	{
	case 0:
		SetRange(200.f);
		coolTime_ = 120;
		GetStatus()->HP.Initialize(30);
		GetStatus()->attack.Initialize(10,10);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 1:
		SetRange(220.f);
		coolTime_ = 110;
		GetStatus()->HP.Initialize(50);
		GetStatus()->attack.Initialize(13, 13);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 2:
		SetRange(250.f);
		coolTime_ = 100;
		GetStatus()->HP.Initialize(70);
		GetStatus()->attack.Initialize(15, 15);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 3:
		SetRange(280.f);
		coolTime_ = 85;
		GetStatus()->HP.Initialize(100);
		GetStatus()->attack.Initialize(18, 18);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 4:
		SetRange(300.f);
		coolTime_ = 70;
		GetStatus()->HP.Initialize(150);
		GetStatus()->attack.Initialize(20, 20);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	}
	SetFov(1000.f);
	box_->setHitBase({-16,-16,32,32});
	SetTarget(ge->playerPtr.get());
	this->render2D_Priority[1] = 0.2f;
}

void RegentLady::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		SetDistance(abs(ML::Vec2(GetPos() - GetTarget()->GetPos()).Length()));
		if (abs(GetFov() > GetDistance()))
		{
			afterState = AIState::Approach;
		}
		break;
	case AIState::Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = AIState::AttackStand;
		}
		break;
	case AIState::AttackStand:
		if (IsAttacking())
		{
			afterState = AIState::Attack;
		}
		break;
	case AIState::Attack:
		if (!IsAttacking())
		{
			afterState = AIState::Approach;
		}
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
			moveCnt_->SetCountFrame(coolTime_);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(0);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void RegentLady::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();
	ML::Vec2 est;
	switch (GetNowState())
	{
	case AIState::Idle:
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
	case AIState::Dead:
		UpDateDead();
		break;
	}
	est=GetMoveVec();
	CheckMove(est);

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

void RegentLady::UpDateApproach()
{
	AIMove_->MoveTo(GetTarget());
}

void RegentLady::UpDateAttackStand()
{
	SetMoveVecX(0);
	if (!moveCnt_->IsCounting())
	{
		BeginAttack();
	}
}

void RegentLady::UpDateAttack()
{
	//ƒŠ[ƒ[ƒ“ƒg‚ð”ò‚Î‚·
	auto rs = RegentShot::Object::Create(true);
	rs->angle_LR_ = angle_LR_;
	rs->SetDamage(GetStatus()->attack.GetNow());
	rs->SetPos(GetPos());
	rs->SetMovementAmount(3.f);

	EndAttack();
}

void RegentLady::UpDateDead()
{
	if (!moveCnt_->IsCounting())
	{
		Kill();
	}
}


