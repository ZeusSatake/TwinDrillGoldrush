#include "NormalLady.h"
#include "../Actors/Task_Player.h"
#include "../System/Task_Save.h"
NormalLady::NormalLady()
	:Lady()
	, standCnt_(60)
	, attackCnt_(90)
	, adjustRange_(30.f)
{
	AddComponent(fanEdge_=shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));

	auto save = ge->GetTask<Save::Object>(Save::defGroupName, Save::defName);
	switch (save->GetValue<int>(Save::Object::ValueKind::StageNo))
	{
	case 0:
		coolTime_ = 60;
		GetStatus()->HP.Initialize(30);
		GetStatus()->attack.Initialize(15, 100);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 1:
		coolTime_ = 50;
		GetStatus()->HP.Initialize(50);
		GetStatus()->attack.Initialize(18, 100);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 2:
		coolTime_ = 50;
		GetStatus()->HP.Initialize(70);
		GetStatus()->attack.Initialize(20, 100);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 3:
		coolTime_ = 45;
		GetStatus()->HP.Initialize(100);
		GetStatus()->attack.Initialize(23, 100);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	case 4:
		coolTime_ = 40;
		GetStatus()->HP.Initialize(150);
		GetStatus()->attack.Initialize(25, 100);
		GetStatus()->defence.Initialize(0, 100);
		GetStatus()->speed.Initialize(2.5f, 100.f, 10.f);
		break;
	}
	gravity_->SetDirection(ML::Vec2::Down());
	gravity_->SetSpeed(0.0f, status_->speed.GetFallSpeed(), 0.5f);
	gravity_->SetAcceleration(ML::Gravity(32) * 10);

	angle_LR_ = Angle_LR::Left;

	SetPreState(AIState::Idle);
	SetNowState(AIState::Idle);

	SetFov(1000.f);
	SetRange(30.f);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(30);
	fanEdge_->setHitBase(ML::Box2D{ -4, -16, 8, 32 });

	SetTarget(ge->playerPtr.get());
	this->render2D_Priority[1] = 0.3f;
	SetPersonalName("お嬢A");//仮
	box_->setHitBase(ML::Box2D{ -16, -16, 32, 32 });
}

void NormalLady::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		SetDistance(abs(ML::Vec2(GetPos() - GetTarget()->GetPos()).Length()));
		if (abs(GetFov()>GetDistance()))
		{
			afterState = AIState::Approach;
		}
		break;
	case AIState::Approach:
		if (WithinRange(GetTarget()))
		{
			afterState = AIState::AttackStand;
		}
		
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
	case AIState::Dead:
		break;
	}
	if (status_->HP.GetNowHP() <= 0)
	{
		afterState = AIState::Dead;
	}
	//状態の更新と各状態ごとの行動カウンタを設定
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::AttackStand:
			moveCnt_->SetCountFrame(coolTime_);
			break;
		case AIState::Attack:
			moveCnt_->SetCountFrame(attackCnt_);
			break;
		default:
			moveCnt_->SetCountFrame(0);
			break;
		}
		moveCnt_->Start();
	}
}

void NormalLady::Move()
{
	moveCnt_->Update();
	unHitTimer_->Update();
	ML::Vec2 est;

	//重力加速
	if (!CheckFoot())
	{

		gravity_->Accel();
	}
	else
	{
		SetMoveVecY(0.f);
	}

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
	case AIState::Damage:
		UpDateDamage();
		break;
	case AIState::Dead:
		UpDateDead();
		break;
	}

	est = GetMoveVec();
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

void NormalLady::UpDatePatrol()
{

}

void NormalLady::UpDateApproach()
{
	AIMove_->MoveTo(GetTarget());
}

void NormalLady::UpDateJump()
{

}

void NormalLady::UpDateFall()
{

}

void NormalLady::UpDateAttackStand()
{
	SetMoveVecX(0);
	if (!moveCnt_->IsCounting())
	{
		BeginAttack();
	}
}

void NormalLady::UpDateAttack()
{

	//攻撃中なら斬撃判定を出す
	if (IsAttacking())
	{
		SetMoveVecX(0);
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());

		
		//左右の斬撃判定の位置を調整
		if (angle_LR_ == Angle_LR::Left)
		{
			attackPos_ = { GetPos().x - adjustRange_, GetPos().y };
		}
		else
		{
			attackPos_ = { GetPos().x + adjustRange_, GetPos().y };
		}
		ML::Box2D fanBox = fanEdge_->getHitBase();
		fanBox.Offset(attackPos_);

		if (fanBox.Hit(plBox))
		{
			static_cast<Player*>(GetTarget())->TakeAttack(status_->attack.GetNow());
		}
		if (!moveCnt_->IsCounting())
		{
			EndAttack();
			return;
		}
	}
}

void NormalLady::UpDateGuard()
{

}

void NormalLady::UpDateDodge()
{

}

void NormalLady::UpDateDamage()
{
	if (!unHitTimer_->IsCounting())
	{
		status_->HP.TakeDamage(ge->playerPtr->GetStatus()->attack.GetNow());
		unHitTimer_->Start();
	}
	if (moveCnt_->IsCounting())
	{
		AIMove_->KnockBack();
	}
	
}

void NormalLady::UpDateDead()
{
	
	if (!moveCnt_->IsCounting())
	{
		this->Kill();
	}
}

float NormalLady::GetAdjustRange() const
{
	return adjustRange_;
}