#include "Player.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"

Player::Player()
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}

void Player::PlayerMove(ML::Vec2 vec)
{
	this->pos_ += vec;
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
		if (map->CheckHit(footBox.OffsetCopy(this->pos_)))
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
		if (map->CheckHit(headBox.OffsetCopy(this->pos_)))
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

	//横軸に対する移動
	while (e_.x != 0) {
		float  preX = this->pos_.x;
		if (e_.x >= 1) { this->pos_.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { this->pos_.x -= 1;		e_.x += 1; }
		else { this->pos_.x += e_.x;		e_.x = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) {
			this->pos_.x = preX;		//移動をキャンセル
			break;
		}
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = this->pos_.y;
		if (e_.y >= 1) { this->pos_.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos_.y -= 1;		e_.y += 1; }
		else { this->pos_.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) {
			this->pos_.y = preY;		//移動をキャンセル
			break;
		}
	}
}

void Player::Think()
{
	auto inp = controller_->gamePad_->GetState();
	switch (pState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		if (true) { pState = StateComponent::State::Walk; }
		break;
	case StateComponent::State::Walk:
		if (inp.B1.down) { pState = StateComponent::State::Jump; }
		if (!CheckFoot()) { pState = StateComponent::State::Fall; }
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
		if (CheckHead()) { pState = StateComponent::State::Fall; }
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
	state_->UpdateNowState(pState);
}

void Player::Move()
{
	ML::Vec2 preVec{0,0};
	auto inp = this->controller_->gamePad_->GetState();

	//if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
	//{
	//	if (map->CheckHit(this->box_->getHitBase().OffsetCopy(this->pos_)))
	//	{
	//		preVec.x += 1;
	//	}
	//}


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
		preVec+=controller_->GetLStickVec();
		
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
		preVec += controller_->GetLStickVec();
		preVec.y -= 5;
		break;
	case StateComponent::State::Fall:
		preVec += controller_->GetLStickVec();
		preVec.y += 5;
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
	CheckMove(preVec);
}

ML::Vec2 Player::GetPos()
{
	return this->pos_;
}

ML::Vec2 Player::Gravity()
{
	return ML::Vec2{ 0,0 };
}