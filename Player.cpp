#include "Player.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
#include "Drill.h"
#include "Source/Components/HPBarComponent.h"

#include "Source/Components/Animation/AnimManager.h"

#include "Source/System/Task_Save.h"


Player::Player()
	:
	moveVec(ML::Vec2{ 0,0 }),
	externalMoveVec(ML::Vec2{0,0}),
	jumpPow(-60.f),
	extCheckFoot(false)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
	AddComponent(cooldown = shared_ptr<TimerComponent>(new TimerComponent(this)));
	AddComponent(overheat = shared_ptr<TimerComponent>(new TimerComponent(this)));
	AddComponent(status_ = shared_ptr<StatusComponent>(new StatusComponent(this)));

	AddComponent(hpBar_ = shared_ptr<HPBarComponent>(new HPBarComponent(this)));

	save_ = Save::Object::Create(true);

	this->cooldown->SetCountFrame(30);
	this->overheat->SetCountFrame(300);
	this->unHitTimer_->SetCountFrame(120);

	this->box_->setHitBase(ML::Box2D{ -8,-8,16,16 });

	this->angle_LR_ = Angle_LR::Left;
	
	status_->HP.Initialize(1000);
	status_->attack.Initialize(100,100);
	status_->speed.Initialize(2.f, 2.f, 2.f);
	status_->defence.Initialize(0, 100);


	//HPバー設定
	hpBar_->SetVisible(true);
	hpBar_->SetSupportScroll(false);
	ML::Point hpBarSize{ 350, 60 };
	hpBar_->SetDrawSize(hpBarSize.x, hpBarSize.y);
	hpBar_->SetPos(hpBarSize.x * 0.5f, ge->screenHeight - hpBarSize.y * 0.5f);
	
	hpBar_->SetImg("./data/image/ui/hpBar/GUISprite_x4.png");
	hpBar_->SetBackSrc  (ML::Box2D(320, 192, 256, 64));
	hpBar_->SetInsideSrc(ML::Box2D(320, 128, 256, 64));
}

bool Player::CheckFoot()
{
	ML::Box2D footBox{
			this->box_->getHitBase().x,
			this->box_->getHitBase().y + this->box_->getHitBase().h,
			this->box_->getHitBase().w,
			1
	};
	if (auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName))
	{
		if (map->CheckHit(footBox.OffsetCopy(this->GetPos())))
		{
			return true;
		}
	}
	return false;
}

void Player::ExtCheckFoot(ML::Box2D target_)
{
	ML::Box2D footBox{
		this->box_->getHitBase().x,
			this->box_->getHitBase().y + this->box_->getHitBase().h,
			this->box_->getHitBase().w,
			1
	}; 
	ML::Box2D me = footBox.OffsetCopy(this->GetPos());
		this->SetExtCheckFoot(me.Hit(target_));
	
}

bool Player::CheckHead()
{
	ML::Box2D headBox{
		this->box_->getHitBase().x,
			this->box_->getHitBase().y - 1,
			this->box_->getHitBase().w,
			1
	};
	if (auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName))
	{
		if (map->CheckHit(headBox.OffsetCopy(this->GetPos())))
		{
			return true;
		}
	}
	return false;
}

void Player::Think()
{
	auto inp = controller_->gamePad_->GetState();
	this->cooldown->Update();

	switch (pState)
	{
	case StateComponent::State::Non:

		break;
	case StateComponent::State::Idle:
		if (inp.LStick.volume != 0) { pState = StateComponent::State::Walk; }
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if (inp.B2.down) { pState = StateComponent::State::Drill; }
		if (inp.L1.down) { pState = StateComponent::State::Attack; }
		break;
	case StateComponent::State::Walk:
		if (inp.LStick.volume == 0) { pState = StateComponent::State::Idle; }
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if (inp.B1.down) { pState = StateComponent::State::Dash; }
		if (inp.B2.down) { pState = StateComponent::State::Drill; }
		if (inp.Trigger.L2.down) { pState = StateComponent::State::SpinAttack; }
		if (inp.L1.down) { pState = StateComponent::State::Attack; }
		if (!CheckFoot() && !CheckHead() &&!extCheckFoot) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Attack:
		if (this->state_->moveCnt_ > 30
			|| inp.L1.off) {
			pState = StateComponent::State::Idle;
		}
		break;
	case StateComponent::State::SpinAttack:
		if (this->drill_->SpinAngle(0.3f)) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Damage:
		if(this->state_->moveCnt_>30)pState = StateComponent::State::Idle;
		break;
	case StateComponent::State::KnockBack:
		if (this->externalMoveVec == ML::Vec2{ 0,0 }||this->state_->moveCnt_ >60)
		{
			pState = StateComponent::State::Idle;
		}
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		if (GetMoveVec().y > 0 || CheckHead()
			|| !CheckFoot()||!extCheckFoot) {
			pState = StateComponent::State::Fall;
		}
		break;
	case StateComponent::State::Fall:
		if (CheckFoot()||extCheckFoot) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Dash:
		if (inp.LStick.volume == 0 || this->state_->moveCnt_ >= 30) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Drill:
		if (inp.L1.down) { pState = StateComponent::State::Mining; }
		if (inp.B2.down) { pState = StateComponent::State::Idle; }
		if (inp.Trigger.L2.down && this->CheckFoot()) { pState = StateComponent::State::DrillDash; }
		break;
	case StateComponent::State::DrillDash:
		if (this->state_->moveCnt_ >= 30) { pState = StateComponent::State::Drill; }
		break;
	case StateComponent::State::Mining:
		if (inp.L1.off) { pState = StateComponent::State::Drill; }
		break;
	case StateComponent::State::Appeal:
		break;
	}	
	if (this->status_->HP.GetNowHP()<=0)
	{
		this->pState=StateComponent::State::Dead;
	}
	this->state_->UpdateNowState(pState);
}

void Player::Move()
{
	auto inp = this->controller_->gamePad_->GetState();
	ML::Vec2 preVec;
	this->unHitTimer_->Update();
	this->cooldown->Update();
	this->overheat->Update();
	this->drill_->UpdateMode(state_->GetNowState());
	

	if (this->overheat->IsCounting())
	{
		if(this->overheat->GetCount()<=1)
		this->drill_->ResetDurability();
	}

	if (this->moveVec.y <= 0 || !CheckHead() || !CheckFoot()||!extCheckFoot)
	{
		this->moveVec.y = min(this->moveVec.y + ((ML::Gravity(10) + (this->state_->moveCnt_ / 10)) * 3), 35.f);
	}
	else
	{
		this->moveVec.y = 0;

	}
	if (this->moveVec.x < 0)
	{
		this->moveVec.x = min(this->moveVec.x + 0.5f, 0);
	}
	else
	{
		this->moveVec.x = max(this->moveVec.x - 0.5f, 0);
	}
	switch (state_->GetNowState())
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		this->status_->speed.SetMax(2.f);
		break;
	case StateComponent::State::Walk:
		this->moveVec.x=controller_->GetLStickVec().x *this->status_->speed.GetMax();
		break;
	case StateComponent::State::Attack:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		this->HitAttack();
		break;
	case StateComponent::State::SpinAttack:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();

		break;
	case StateComponent::State::Damage:
		

		break;
	case StateComponent::State::KnockBack:
		//this->moveVec.x = externalMoveVec.x;
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		moveVec.y = jumpPow + (this->state_->moveCnt_ / 20);
		break;
	case StateComponent::State::Fall:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		break;
	case StateComponent::State::Dash:
		this->status_->speed.SetMax(5.0f);
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		break;
	case StateComponent::State::DrillDash:
		if (this->state_->moveCnt_ == 0)this->drill_->SetCanRotate(false);
		moveVec = this->drill_->DrillAngleVec() * 5;
		this->drill_->Mining();
		if (this->state_->moveCnt_ >= 29)this->drill_->SetCanRotate(true);
		break;
	case StateComponent::State::Drill:
		this->status_->speed.SetMax(0.85f);
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		if (inp.R1.down && this->CheckFoot())
		{
			moveVec.y = jumpPow - 10 + (this->state_->moveCnt_ / 10);
			this->state_->moveCnt_ = 0;
		}
		break;
	case StateComponent::State::Mining:

		if (this->cooldown->GetCount() <= 0)
		{
			if(this->UpdateDrilldurability())
			this->drill_->Mining();
			this->cooldown->Start();
		}
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		break;
	case StateComponent::State::Appeal:
		break;
	}
	if (this->controller_->GetLStickVec().x < 0)
	{
		this->angle_LR_ = Angle_LR::Left;
	}
	else if(this->controller_->GetLStickVec().x>0)
	{
		this->angle_LR_ = Angle_LR::Right;
	}
    //this->CheckHitMap(this->preVec);
	//externalMoveVec.y = 0;
	
	if (externalMoveVec.x !=0)
	{
		moveVec.x = moveVec.x * 0.05;
	}

	CheckMove(moveVec);
	CheckMove(externalMoveVec);
}

void Player::CollisionJudge(ML::Box2D hitBox_ ,ML::Vec2 pos_)
{
	
	ML::Rect ext =
	{
		hitBox_.OffsetCopy(pos_).x,
		hitBox_.OffsetCopy(pos_).y,
		hitBox_.OffsetCopy(pos_).x + hitBox_.OffsetCopy(pos_).w,
		hitBox_.OffsetCopy(pos_).y + hitBox_.OffsetCopy(pos_).h
	};

	ML::Rect me =
	{
		this->box_->getHitBase().OffsetCopy(this->GetPos()).x,
		this->box_->getHitBase().OffsetCopy(this->GetPos()).y,
		this->box_->getHitBase().OffsetCopy(this->GetPos()).x + this->box_->getHitBase().OffsetCopy(this->GetPos()).w,
		this->box_->getHitBase().OffsetCopy(this->GetPos()).y + this->box_->getHitBase().OffsetCopy(this->GetPos()).h,
	};

	if(CheckHit(hitBox_.OffsetCopy(pos_)))
	{
	    //this->state_->UpdateNowState(StateComponent::State::KnockBack);
		if (ext.left - 1 < me.right) { this->externalMoveVec.x = -1; }
		if (ext.bottom > me.top) { this->externalMoveVec.y = 1; }
		if (ext.right > me.left) { this->externalMoveVec.x = 1; }
		if (ext.top < me.bottom - 1) { this->externalMoveVec.y = -1; this->moveVec.y = 0; }
	}
}

bool Player::UpdateDrilldurability()
{
	if (this->drill_->GetDurability() <= 0)
	{
		return false;
	}
	else
	{
		this->drill_->SetDurability(this->drill_->GetDurability() - 1);
		if (this->drill_->GetDurability() <= 0)
		{
			this->overheat->Start();
		}
		return true;
	}
	return false;
}

void Player::SetDrillDurability(int durability_)
{
	this->drill_->SetDurability(durability_);
}

void Player::HitAttack()
{
	this->cooldown->Start();
}

void Player::TakeAttack(int damage_)
{
	int damage = damage_ - this->status_->defence.GetNow();
	if (damage < 0) { damage = 0; }
	if (!this->unHitTimer_->IsCounting())
	{
		this->status_->HP.TakeDamage(damage);
		this->unHitTimer_->Start();
		this->state_->UpdateNowState(StateComponent::State::Damage);
	}
}

void Player::SetPlayerState(StateComponent::State state)
{
	if (this->state_->GetNowState() == state) { return; }
	this->state_->UpdateNowState(state);
}

void Player::SetExternalVec(ML::Vec2 moveVec_)
{
	this->externalMoveVec = moveVec_;
	//this->state_->UpdateNowState(StateComponent::State::KnockBack);
}

int Player::GetDrillAttack()
{
	return this->drill_->GetAttack();
}
ML::Box2D Player::GetAttackBox()
{
	this->AttackBox = ML::Box2D{ -8,-8,16,16 };
	this->AttackBox.Offset(drill_->GetAttackPos());
	return this->AttackBox;
}

ML::Vec2 Player::GetMoveVec()
{
	return this->moveVec;
}


StatusComponent* Player::GetStatus() const
{
	return this->status_.get();
}

HPBarComponent* Player::GetHPBar() const
{
	return this->hpBar_.get();
}

ML::Vec2 Player::GetExternalVec()
{
	return this->externalMoveVec;
}

void Player::ResetState()
{
	this->state_->UpdateNowState(StateComponent::State::Idle);
	this->pState = this->state_->GetNowState();
	this->drill_->SetPos(this->GetPos());
	this->drill_->SetMode(Drill::Mode::Normal);
}

void Player::HiddenPlayer()
{
	this->state_->UpdateNowState(StateComponent::State::Non);
	this->drill_->SetMode(Drill::Mode::Non);
}

void Player::UpdateStates()
{
	//save_->GetValue<int>(Save::Object::ValueKind::DefenceLevel);

}

void Player::SetAnim()
{
	this->animManager_->SetDefaultAnimId((int)StateComponent::State::Idle);
	ML::Color defColor{ 1,1,1,1 };
	ML::Box2D drawSize = this->box_->getHitBase();
	AnimInfo animTable[] =
	{
		{drawSize,ML::Box2D{0,0,64,64},defColor,2,8},
		{drawSize,ML::Box2D{0,64,64,64},defColor,6,4},
		{drawSize,ML::Box2D{0,128,64,64},defColor,6,4},
		{drawSize,ML::Box2D{0,192,64,64},defColor,5,4},
		{drawSize,ML::Box2D{0,256,64,64},defColor,1,1},
	};
	this->animManager_->AddAnim((int)StateComponent::State::Idle, animTable[0]);
	this->animManager_->AddAnim((int)StateComponent::State::Walk, animTable[1]);
	this->animManager_->AddAnim((int)StateComponent::State::Dash, animTable[2]);
	this->animManager_->AddAnim((int)StateComponent::State::Jump, animTable[3]);
	this->animManager_->AddAnim((int)StateComponent::State::Fall, animTable[4]);
}

void Player::SetExtCheckFoot(bool Check_)
{
	this->extCheckFoot = Check_;
}

bool Player::GetExtCheckFoot()
{
	return this->extCheckFoot;
}