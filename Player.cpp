#include "Player.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"

Player::Player()
	:
	moveVec(ML::Vec2{0,0})
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
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

void Player::CheckMove(ML::Vec2& e_)
{
	//マップが存在するか調べてからアクセス
	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return; }//マップが無ければ判定しない(出来ない）
	ML::Vec2 preVec{ 0,0 };
	//横軸に対する移動
	while (e_.x != 0) {
		float  preX = this->GetPos().x;
		if (e_.x >= 1) { SetPosX(GetPos().x +1);	e_.x -= 1; }
		else if (e_.x <= -1) { SetPosX(GetPos().x - 1);		e_.x += 1; }
		else {SetPosX(GetPos().x + e_.x);		e_.x = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
		if (true == map->CheckHit(hit)) {
			SetPosX(preX);
					//移動をキャンセル
			break;
		}
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = this->GetPos().y;
		if (e_.y >= 1) {SetPosY(GetPos().y + 1);		e_.y -= 1; }
		else if (e_.y <= -1) { SetPosY(GetPos().y -1);		e_.y += 1; }
		else { SetPosY( GetPos().y + e_.y);		e_.y = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
		if (true == map->CheckHit(hit)) {
			this->SetPosY(preY);
					//移動をキャンセル
			break;
		}
	}
	
}


void Player::ResetCnt()
{
	this->moveCnt_ = 0;
	state_->UpdateNowState(pState);
}

void Player::Think()
{
	auto inp = controller_->gamePad_->GetState();
	switch (pState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		if (inp.LStick.volume!=0) { pState = StateComponent::State::Walk; }
		if (inp.B1.down) { pState = StateComponent::State::Jump; }
		break;
	case StateComponent::State::Walk:
		if (inp.B1.down) { pState = StateComponent::State::Jump; }
		if (!CheckFoot()&&!CheckHead()) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Attack:
		break;
	case StateComponent::State::SpinAttack:
		break;
	case StateComponent::State::Damage:
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		if (this->moveVec.y > 0 || CheckHead()
			||!CheckFoot()) { pState = StateComponent::State::Fall; }
		break;
	case StateComponent::State::Fall:
		if (CheckFoot()) { pState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
		break;
	case StateComponent::State::Appeal:
		break;
	}
	this->ResetCnt();
}

void Player::Move()
{
	auto inp = this->controller_->gamePad_->GetState();

	//if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
	//{
	//	if (map->CheckHit(this->box_->getHitBase().OffsetCopy(this->pos_)))
	//	{
	//		preVec.x += 1;
	//	}
	//}
	if (this->moveVec.y<0||!CheckHead()||!CheckFoot())
	{
		this->moveVec.y = min(this->moveVec.y+ML::Gravity(35)*5+(moveCnt_/10), 25.f);
	}
	else
	{
		this->moveVec.y = 0;
	}

	switch (state_->GetNowState())
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:

		break;
	case StateComponent::State::Walk:
		/*if (inp.LStick.BU.on) { preVec.y -= 3; }
		if (inp.LStick.BD.on) { preVec.y += 3; }
		if (inp.LStick.BR.on) { preVec.x += 3; }
		if (inp.LStick.BL.on) { preVec.x -= 3; }*/
		moveVec.x=controller_->GetLStickVec().x;
		break;
	case StateComponent::State::Attack:
		break;
	case StateComponent::State::SpinAttack:
		break;
	case StateComponent::State::Damage:
		break;
	case StateComponent::State::KnockBack:
		break;
	case StateComponent::State::Dead:
		break;
	case StateComponent::State::Jump:
		moveVec.x = controller_->GetLStickVec().x;
		moveVec.y = -25.f + (this->moveCnt_/10);
		break;
	case StateComponent::State::Fall:
		moveVec.x = controller_->GetLStickVec().x;
		
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
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