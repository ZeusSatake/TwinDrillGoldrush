#include "LadyKiyohara.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Task_Bomb.h"

LadyKiyohara::LadyKiyohara()
	:BossLady()
	, isAttacking_(false)
	, defaultFlyPosY_(200.f)
	, attackPattern_(AttackPattern::Non)
	, toGlidingPos_(0.f, 0.f)
	, toVec_(0.f, 0.f)
	, bombDistance_(50.f)
{
	SetFov(1000.f);
	box_->setHitBase(ML::Box2D{ -8, -16, 16, 32 });
	GetStatus()->HP.Initialize(250);
	preHP_=GetStatus()->HP.GetNowHP();
	GetStatus()->speed.Initialize(3.5f, 7.f, -5.f);

	moveCnt_->SetCountFrame(0);
	unHitTimer_->SetCountFrame(15);

	SetTarget(ge->playerPtr.get());
}

void LadyKiyohara::Think()
{
	AIState afterState = GetNowState();
	switch (afterState)
	{
	case AIState::Idle:
		if (WithinSight(GetTarget()))//�C�x���g�I�����Ă���؂�ւ�
		{
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
		break;
	case AIState::AttackStand:
		if (!moveCnt_->IsCounting())
		{
			switch (attackPattern_)
			{
			case AttackPattern::Non:
				attackPattern_ = AttackPattern::DropBombs;
				break;
			case AttackPattern::DropBombs:
				attackPattern_ = AttackPattern::DropBombs;
				break;
			case AttackPattern::GlidingAttack:
				attackPattern_ = AttackPattern::TackleAttack;
				break;
			case AttackPattern::TackleAttack:
				attackPattern_ = AttackPattern::DropBombs;
				break;
			}
			afterState = AIState::Attack;
		}
		break;
	case AIState::Attack:
		if (!isAttacking_)
		{
			switch (attackPattern_)
			{
			case AttackPattern::DropBombs:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::GlidingAttack:
				afterState = AIState::AttackStand;
				break;
			case AttackPattern::TackleAttack:
				afterState = AIState::Fly;
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
		if (GetPos().y <= defaultFlyPosY_)
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
	//��Ԃ̍X�V�Ɗe��Ԃ��Ƃ̍s���J�E���^��ݒ�
	if (UpDateState(afterState))
	{
		switch (afterState)
		{
		case AIState::AttackStand:
			moveCnt_->SetCountFrame(120);
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

	//�_���[�W����
	if (ge->playerPtr->pState == StateComponent::State::Attack && !unHitTimer_->IsCounting())
	{
		ML::Box2D plBox = GetTarget()->GetBox()->getHitBase();
		plBox.Offset(GetTarget()->GetPos());
		if (box_->CheckHit(plBox))
		{
			ge->playerPtr->TakeAttack(status_->attack.GetNow());
		}
	}

	if (preHP_ != GetStatus()->HP.GetNowHP())
	{
		preHP_ = GetStatus()->HP.GetNowHP();
		unHitTimer_->Start();
	}
}

void LadyKiyohara::UpDateFly()
{
	SetMoveVecY(GetStatus()->speed.GetNow());
}

void LadyKiyohara::UpDateDamage()
{

}

void LadyKiyohara::UpDateDead()
{
	Kill();
}

void LadyKiyohara::UpDateAttackStand()
{
	switch (attackPattern_)
	{
	case AttackPattern::Non:
		break;
	case AttackPattern::DropBombs:
		SetMoveVecX(ge->playerPtr->GetPos().x - GetPos().x);
		SetMoveVecY(0);
		break;
	case AttackPattern::GlidingAttack:
		//�~���������������
		SetMoveVec(ML::Vec2{0, 0});
		toVec_ = CalcAngle(ge->playerPtr->GetPos());
		toGlidingPos_ = ge->playerPtr->GetPos();
		break;
	case AttackPattern::TackleAttack:
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
}

void LadyKiyohara::UpDateGlidingAttack()
{
	SetMoveVec(toVec_ * status_->speed.GetNow());
	//���莞��Y���W�܂ō~��������U���I��
	if (toGlidingPos_.y < GetPos().y)
	{
		isAttacking_ = false;
	}
}

void LadyKiyohara::UpDateTackleAttack()
{

}

