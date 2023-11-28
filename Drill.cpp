#include "Drill.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
Drill::Drill()
	:
	attackPoint(0),
	nowAngle(0.0f),
	durability(0)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}

void Drill::InitPos(ML::Vec2 pos)
{
	this->pos_ = pos;
}

void Drill::SetAngle(float angle)
{
	this->angle_ = angle;
}

int Drill::GetAttackPoint()
{
	return this->attackPoint;
}

float Drill::GetNowAngle()
{
	return this->nowAngle;
}

ML::Vec2 Drill::GetDrillPoint()
{
	return this->pos_;
}

float Drill::UpdateDrillAngle()
{
	auto inp = controller_->gamePad_->GetState();
	
	return inp.RStick.angleDYP;
	/*if (inp.RStick.BU.on) { return ML::ToRadian(90); }
	else return 0;*/
}

void Drill::Mining()
{
	if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
	{
		ML::Vec2 preVec{
			this->pos_.x - ge->camera2D.x,
				this->pos_.y -ge->camera2D.y
		};
		map->Search(preVec);
	}
}

void Drill::DrillCheckMove(ML::Vec2 e_)
{
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
}


void Drill::Think()
{
	auto inp = controller_->gamePad_->GetState();

	switch (dState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		if (inp.B2.down) { dState = StateComponent::State::Mining; }
		if (inp.RStick.volume != 0) { dState = StateComponent::State::Walk; }
		break;
	case StateComponent::State::Walk:
		if (inp.B2.down) { dState = StateComponent::State::Mining; }
		if (inp.RStick.volume == 0) { dState = StateComponent::State::Idle; }
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
		break;
	case StateComponent::State::Fall:
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
		if (inp.B2.off) { dState = StateComponent::State::Idle; }
		break;
	case StateComponent::State::Appeal:
		break;
	}
	state_->UpdateNowState(dState);
}

void Drill::Move()
{
	auto inp = controller_->gamePad_->GetState();
	switch (dState)
	{
	case StateComponent::State::Non:
		break;
	case StateComponent::State::Idle:
		break;
	case StateComponent::State::Walk:
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
		break;
	case StateComponent::State::Fall:
		break;
	case StateComponent::State::Dash:
		break;
	case StateComponent::State::DrillDash:
		break;
	case StateComponent::State::Mining:
		Mining();

		break;
	case StateComponent::State::Appeal:
		break;

	}
}