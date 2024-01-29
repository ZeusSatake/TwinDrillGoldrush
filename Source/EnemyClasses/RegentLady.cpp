#include "RegentLady.h"

#include "../Actors/Task_Player.h"
#include "../System/Task_Save.h"

RegentLady::RegentLady()
	:Lady()
	, coolTime_(0)
{
	auto save = ge->GetTask<Save::Object>(Save::defGroupName, Save::defName);
	switch (save->GetValue<int>(Save::Object::ValueKind::StageNo))
	{
	case 1:
		coolTime_ = 60;
		GetStatus()->HP.Initialize(30);
		GetStatus()->attack.Initialize(5,5);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 2:
		coolTime_ = 50;
		GetStatus()->HP.Initialize(50);
		GetStatus()->attack.Initialize(7, 7);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 3:
		coolTime_ = 50;
		GetStatus()->HP.Initialize(70);
		GetStatus()->attack.Initialize(10, 10);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 4:
		coolTime_ = 45;
		GetStatus()->HP.Initialize(100);
		GetStatus()->attack.Initialize(15, 15);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 5:
		coolTime_ = 40;
		GetStatus()->HP.Initialize(150);
		GetStatus()->attack.Initialize(20, 20);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	}
	box_->setHitBase({-16,-16,32,32});
	SetTarget(ge->playerPtr.get());
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
}

void RegentLady::UpDateApproach()
{
	AIMove_->MoveTo(GetTarget());
}

void RegentLady::UpDateAttackStand()
{
}

void RegentLady::UpDateAttack()
{
	//ƒŠ[ƒ[ƒ“ƒg‚ğ”ò‚Î‚·

}

void RegentLady::UpDateDead()
{
}


