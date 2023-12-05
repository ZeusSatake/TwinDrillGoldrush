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
			this->GetPos().x - ge->camera2D.x/*+(cos(this->UpdateDrillAngle()) * 5.f)*/,
				this->GetPos().y - ge->camera2D.y /*+(sin(this->UpdateDrillAngle()) * 5.0f)*/
		};
		map->Search(preVec);
	}
}

void Drill::DrillCheckMove(ML::Vec2 e_)
{
	//マップが存在するか調べてからアクセス
	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return; }//マップが無ければ判定しない(出来ない）
	ML::Vec2 preVec{ 0,0 };
	//横軸に対する移動
	while (e_.x != 0) {
		float  preX = this->GetPos().x;
		if (e_.x >= 1) { SetPosX(GetPos().x + 1);	e_.x -= 1; }
		else if (e_.x <= -1) { SetPosX(GetPos().x - 1);		e_.x += 1; }
		else { SetPosX(GetPos().x + e_.x);		e_.x = 0; }
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
		if (e_.y >= 1) { SetPosY(GetPos().y + 1);		e_.y -= 1; }
		else if (e_.y <= -1) { SetPosY(GetPos().y - 1);		e_.y += 1; }
		else { SetPosY(GetPos().y + e_.y);		e_.y = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
		if (true == map->CheckHit(hit)) {
			this->SetPosY(preY);
			//移動をキャンセル
			break;
		}
	}
}