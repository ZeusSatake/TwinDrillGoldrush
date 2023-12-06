#include "Drill.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
Drill::Drill()
	:
	attackPoint(0),
	angle(0.0f),
	preAngle(0.0f),
	durability(0),
	canRotate(true)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}



void Drill::SetAngle(float angle)
{
	this->angle = angle;
}

void Drill::SetCanRotate(bool check)
{
	this->canRotate = check;
}

int Drill::GetAttackPoint()
{
	return this->attackPoint;
}

float Drill::GetNowAngle()
{
	return this->angle;
}


float Drill::UpdateDrillAngle()
{
	auto inp = controller_->gamePad_->GetState();
	this->preAngle =inp.RStick.angleDYP;
	if (canRotate)
	{
		if (this->angle != preAngle && preAngle != 0)
		{
			this->angle = this->preAngle;
		}
	}
	return this->angle;
}

void Drill::Mining()
{
	if (auto map = ge->GetTask<Map::Object>("�{��", "�}�b�v"))
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
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) { return; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	ML::Vec2 preVec{ 0,0 };
	//�����ɑ΂���ړ�
	while (e_.x != 0) {
		float  preX = this->GetPos().x;
		if (e_.x >= 1) { SetPosX(GetPos().x + 1);	e_.x -= 1; }
		else if (e_.x <= -1) { SetPosX(GetPos().x - 1);		e_.x += 1; }
		else { SetPosX(GetPos().x + e_.x);		e_.x = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
		if (true == map->CheckHit(hit)) {
			SetPosX(preX);
			//�ړ����L�����Z��
			break;
		}
	}
	//�c���ɑ΂���ړ�
	while (e_.y != 0) {
		float  preY = this->GetPos().y;
		if (e_.y >= 1) { SetPosY(GetPos().y + 1);		e_.y -= 1; }
		else if (e_.y <= -1) { SetPosY(GetPos().y - 1);		e_.y += 1; }
		else { SetPosY(GetPos().y + e_.y);		e_.y = 0; }
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->GetPos());
		if (true == map->CheckHit(hit)) {
			this->SetPosY(preY);
			//�ړ����L�����Z��
			break;
		}
	}
}