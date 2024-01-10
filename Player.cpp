#include "Player.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
#include "Drill.h"

Player::Player()
	:
	moveVec(ML::Vec2{ 0,0 }),
	jumpPow(-25.f),
	PreHp(100)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
	AddComponent(cooldown_ = shared_ptr<TimerComponent>(new TimerComponent(this)));
	AddComponent(status_ = shared_ptr<StatusComponent>(new StatusComponent(this)));
	//this->movement_->SetConsiderationCollition(true);
	//this->gravity_->SetConsiderationCollition(true);
	this->cooldown_->SetCountFrame(30);
	this->unHitTimer_->SetCountFrame(30);
	
	status_->HP.Initialize(1000);
	this->PreHp = this->status_->HP.GetNowHP();
	status_->attack.Initialize(10,100);
	status_->speed.Initialize(2.f, 2.f, 2.f);
	status_->defence.Initialize(0, 100);
}



bool Player::CheckFoot()
{
	ML::Box2D footBox{
		    this->box_->getHitBase().x,
			this->box_->getHitBase().y +this->box_->getHitBase().h,
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

//void Player::CheckMove(ML::Vec2& e_)
//{
//	//マップが存在するか調べてからアクセス
//	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
//	if (nullptr == map) { return; }//マップが無ければ判定しない(出来ない）
//	ML::Vec2 preVec{ 0,0 };
//	//横軸に対する移動
//	while (e_.x != 0) {
//		float  preX = this->GetPos().x;
//		if (e_.x >= 1) { SetPosX(GetPos().x +1);	e_.x -= 1; }
//		else if (e_.x <= -1) { SetPosX(GetPos().x - 1);		e_.x += 1; }
//		else {SetPosX(GetPos().x + e_.x);		e_.x = 0; }
//		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
//		if (true == map->CheckHit(hit)) {
//			SetPosX(preX);
//					//移動をキャンセル
//			break;
//		}
//	}
//	//縦軸に対する移動
//	while (e_.y != 0) {
//		float  preY = this->GetPos().y;
//		if (e_.y >= 1) {SetPosY(GetPos().y + 1);		e_.y -= 1; }
//		else if (e_.y <= -1) { SetPosY(GetPos().y -1);		e_.y += 1; }
//		else { SetPosY( GetPos().y + e_.y);		e_.y = 0; }
//		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
//		if (true == map->CheckHit(hit)) {
//			this->SetPosY(preY);
//					//移動をキャンセル
//			break;
//		}
//	}
//	
//}


void Player::Think()
{
	auto inp = controller_->gamePad_->GetState();
	this->cooldown_->Update();

	switch (pState)
	{
	case StateComponent::State::Non:

		break;
	case StateComponent::State::Idle:
		if (inp.LStick.volume!=0) { pState = StateComponent::State::Walk; }
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if (inp.B2.down) { pState = StateComponent::State::Drill; }
		if(inp.L1.down){pState = StateComponent::State::Attack; }
		break;
	case StateComponent::State::Walk:
		if (inp.LStick.volume == 0) { pState = StateComponent::State::Idle; }
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if(inp.B1.down){pState = StateComponent::State::Dash;}
		if(inp.B2.down){ pState = StateComponent::State::Drill; }
		if(inp.Trigger.L2.down){ pState = StateComponent::State::SpinAttack; }
		if(inp.L1.down){ pState = StateComponent::State::Attack; }
		if (!CheckFoot()&&!CheckHead()) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Attack:
		if (this->state_->moveCnt_ > 30
			||inp.L1.off) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::SpinAttack:
		if (this->drill_->SpinAngle(0.3f)){ pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Damage:
		if(this->state_->moveCnt_>90){ pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		if (GetMoveVec().y > 0 || CheckHead()
			||!CheckFoot()) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Fall:
		if (CheckFoot()) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Dash:
		if(inp.LStick.volume ==0||this->state_->moveCnt_ >=30){pState= StateComponent::State::Idle; }
		break;
	case StateComponent::State::Drill:
		if (inp.L1.on) { pState = StateComponent::State::Mining; }
		if (inp.B2.down) { pState = StateComponent::State::Idle; }
		if (inp.Trigger.L2.down&&this->CheckFoot()) { pState = StateComponent::State::DrillDash; }
		break;
	case StateComponent::State::DrillDash:
		if(this->state_->moveCnt_>=30){pState= StateComponent::State::Drill;}
		break;
	case StateComponent::State::Mining:
		if(inp.L1.off) { pState = StateComponent::State::Drill; }
		break;
	case StateComponent::State::Appeal:
		break;
	}
	
	if (this->PreHp != this->status_->HP.GetNowHP())
	{
		this->pState = StateComponent::State::Damage;
		this->PreHp = this->status_->HP.GetNowHP();
	}

	/*if (this->status_->HP.IsAlive())
	{
		this->pState=StateComponent::State::Dead;
	}*/
	this->state_->UpdateNowState(pState);
}

void Player::Move()
{
	auto inp = this->controller_->gamePad_->GetState();
	ML::Vec2 preVec;
	this->unHitTimer_->Update();
	this->drill_->SetMode(state_->GetNowState());

	if (this->moveVec.y<=0||!CheckHead()||!CheckFoot())
	{
		this->moveVec.y = min(this->moveVec.y+((ML::Gravity(25) +(this->state_->moveCnt_/10)) * 5), 35.f);
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
		break;
	case StateComponent::State::Walk:
		this->moveVec.x=controller_->GetLStickVec().x;
		
		break;
	case StateComponent::State::Attack:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		this->HitAttack();
		break;
	case StateComponent::State::SpinAttack:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();

		break;
	case StateComponent::State::Damage:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();

		this->unHitTimer_->Start();
		TakeAttack();
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		moveVec.y = jumpPow + (this->state_->moveCnt_/20);
		break;
	case StateComponent::State::Fall:
		moveVec.x = controller_->GetLStickVec().x* this->status_->speed.GetMax();
		break;
	case StateComponent::State::Dash:
		this->status_->speed.SetMax(5.0f);
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();

		break;
	case StateComponent::State::DrillDash:
		if(this->state_->moveCnt_ == 0)this->drill_->SetCanRotate(false);
		moveVec = this->drill_->DrillAngleVec()*5;
		this->drill_->Mining();
		if (this->state_->moveCnt_ >= 29)this->drill_->SetCanRotate(true);
		break;
	case StateComponent::State::Drill:
		this->status_->speed.SetMax(0.85f);
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();
		if (inp.R1.down&&this->CheckFoot()) 
		{
			moveVec.y = jumpPow-10 + (this->state_->moveCnt_ / 10);
			this->state_->moveCnt_ = 0;
		}
		break;
	case StateComponent::State::Mining:
		this->drill_->Mining();
		moveVec.x = controller_->GetLStickVec().x * this->status_->speed.GetMax();

		break;
	case StateComponent::State::Appeal:
		break;
	}
	//ここに最終的にマップとの移動可否チェックを入れる
    //this->CheckHitMap(this->preVec);
	CheckMove(moveVec);
}


void Player::HitAttack()
{
	this->cooldown_->Start();
}

void Player::TakeAttack()
{
	if (!this->unHitTimer_->IsCounting())
	{
		this->moveVec.y = -0.5;
		this->status_->HP.TakeDamage(1);
	}
}

void Player::SetPlayerState(StateComponent::State state)
{
	if (this->state_->GetNowState() == state) { return; }
	this->state_->UpdateNowState(state);
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

void Player::ResetState()
{
	this->state_->UpdateNowState(StateComponent::State::Idle);
	this->drill_->SetMode(StateComponent::State::Idle);
}

void Player::HiddenPlayer()
{
	this->state_->UpdateNowState(StateComponent::State::Non);
	this->drill_->SetMode(StateComponent::State::Non);
}