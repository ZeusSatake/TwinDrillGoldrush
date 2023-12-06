#include "Player.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
#include "Drill.h"

Player::Player()
	:
	moveVec(ML::Vec2{ 0,0 }),
	speed(2.0f),
	jumpPow(-20.f)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
	//this->movement_->SetConsiderationCollition(true);
	//this->gravity_->SetConsiderationCollition(true);

}



bool Player::CheckFoot()
{
	ML::Box2D footBox{
		    this->box_->getHitBase().x,
			this->box_->getHitBase().y +this->box_->getHitBase().h,
			this->box_->getHitBase().w,
			1
	};
	if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
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
	if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
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
	switch (pState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		if (inp.LStick.volume!=0) { pState = StateComponent::State::Walk; }
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if (inp.B2.down) { pState = StateComponent::State::Drill; }
		break;
	case StateComponent::State::Walk:
		if (inp.R1.down) { pState = StateComponent::State::Jump; }
		if(inp.B1.down){pState = StateComponent::State::Dash;}
		if(inp.B2.down){ pState = StateComponent::State::Drill; }
		if(inp.Trigger.L2.down){ pState = StateComponent::State::SpinAttack; }
		if (!CheckFoot()&&!CheckHead()) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Attack:
		break;
	case StateComponent::State::SpinAttack:
		if (this->drill_->SpinAngle(0.3f)){ pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Damage:
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
		if (inp.Trigger.L2.down) { pState = StateComponent::State::DrillDash; }
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
	this->state_->UpdateNowState(pState);
}

void Player::Move()
{
	auto inp = this->controller_->gamePad_->GetState();
	ML::Vec2 preVec;
	if (this->moveVec.y<=0||!CheckHead()||!CheckFoot())
	{
		this->moveVec.y = min(this->moveVec.y+((ML::Gravity(25) +(this->state_->moveCnt_/10)) * 5), 35.f);
		//gravity_->Accel();
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
		moveVec.x = controller_->GetLStickVec().x * speed;
		break;
	case StateComponent::State::SpinAttack:
		moveVec.x = controller_->GetLStickVec().x * speed;

		break;
	case StateComponent::State::Damage:
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		moveVec.x = controller_->GetLStickVec().x * speed;
		moveVec.y = jumpPow + (this->state_->moveCnt_/10);
		break;
	case StateComponent::State::Fall:
		moveVec.x = controller_->GetLStickVec().x*speed;
		break;
	case StateComponent::State::Dash:
		this->speed = 5.0f;
		moveVec.x = controller_->GetLStickVec().x * speed;

		break;
	case StateComponent::State::DrillDash:
		if(this->state_->moveCnt_ == 0)this->drill_->SetCanRotate(false);
		moveVec = this->drill_->DrillAngleVec()*5;
		this->drill_->Mining();
		if (this->state_->moveCnt_ >= 29)this->drill_->SetCanRotate(true);
		break;
	case StateComponent::State::Drill:
		this->speed = 0.85f;
		moveVec.x = controller_->GetLStickVec().x * speed;
		if (inp.R1.down) 
		{
			moveVec.y = jumpPow-10 + (this->state_->moveCnt_ / 10);
			this->state_->moveCnt_ = 0;
		}
		break;
	case StateComponent::State::Mining:
		this->drill_->Mining();
		moveVec.x = controller_->GetLStickVec().x * speed;
		if (inp.R1.down)
		{
			moveVec.y = jumpPow-10 + (this->state_->moveCnt_ / 10);
			this->state_->moveCnt_ = 0;
		}
		break;
	case StateComponent::State::Appeal:
		break;
	}
	//ここに最終的にマップとの移動可否チェックを入れる
    //this->CheckHitMap(this->preVec);
	CheckMove(moveVec);
}


ML::Vec2 Player::GetMoveVec()
{
	return this->moveVec;
}