#include <math.h>

#include "Drill.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
#include "Source/Scene/Task_JewelryMap.h"
#include "Source/Components/Blocks/BlockManager.h"

Drill::Drill()
	:
	attack(1),
	angle(0.0f),
	addAngle(0.0f),
	preAngle(0.0f),
	durability(30),
	maxDurability(30),
	Length(1.f),
	moveVec(ML::Vec2{0,0}),
	animMove(ML::Vec2{0,0}),
	canRotate(true),
	CheckOverHeat(false)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));

	this->SetDurability(30);
}



void Drill::SetAngle(float angle)
{
	this->angle = angle;
}

void Drill::SetDrawPos(ML::Vec2 pos)
{
	this->drawPos = pos;
}

void Drill::SetMode(Mode mode_)
{
	this->mode = mode_;
}

void Drill::UpdateMode(StateComponent::State state)
{
	if (state == StateComponent::State::Drill ||
		state == StateComponent::State::DrillDash ||
		state == StateComponent::State::Mining)
	{
		this->mode = Mode::Drill;
		return;
	}
	if (state == StateComponent::State::Dead)
	{
		this->mode = Mode::Non;
		return;
	}
	if (state == StateComponent::State::Non)
	{
		this->mode = Mode::Non;
		return;
	}
	this->mode = Mode::Normal;
}

Drill::Mode Drill::GetMode()
{
	return this->mode;
}

void Drill::UpdateTargetPos(ML::Vec2 pos)
{
	this->targetPos = pos;
}

ML::Vec2 Drill::GetDrawPos()
{
	return this->drawPos;
}

ML::Vec2 Drill::GetTargetPos()
{
	return this->targetPos;
}

void Drill::SetCheckOverHeat(bool check)
{
	this->CheckOverHeat = check;
}

bool Drill::GetCheckOverHeat()
{
	return this->CheckOverHeat;
}

bool Drill::SpinAngle(float angle)
{
	
	if (ML::ToDegree(this->addAngle)< 360)
	{
		this->angle += angle;
		addAngle += angle;
	}
	else
	{
		this->addAngle = 0.f;
		return true;
	}
	return false;
}

void Drill::SetCanRotate(bool check)
{
	this->canRotate = check;
}

int Drill::GetAttack()
{
	return this->attack;
}

void Drill::SetAttack(int attack_)
{
	this->attack = attack_;
}

float Drill::GetNowAngle()
{
	return this->angle;
}

float Drill::GetLenght()
{
	return this->Length;
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

ML::Vec2 Drill::DrillAngleVec()
{
	return ML::Vec2
	{
		cos(this->UpdateDrillAngle()),
		sin(this->UpdateDrillAngle())
	};
}

bool Drill::LimitLength(ML::Vec2 pos)
{
	ML::Vec2 diff = ML::Vec2{
		fabsf(this->GetTargetPos().x*16 - pos.x),
		fabsf(this->GetTargetPos().y*16 - pos.y)
	};
	float hypo = (diff.x * diff.x) + (diff.y * diff.y);
	if (this->Length*16*this->Length*16 < hypo)
	{
		return true;
	}
	return false;
}

void Drill::Mining()
{
	auto map = ge->GetTasks<Map::Object>(Map::defGroupName, Map::defName);
	for(auto it = map->begin();it!=map->end();it++)
	{
		ML::Vec2 preVec{
			this->GetPos().x-ge->camera2D.x+(cos(this->UpdateDrillAngle()) * 16.f),
				this->GetPos().y-ge->camera2D.y +(sin(this->UpdateDrillAngle()) * 16.f)
		};
		it->get()->Search(preVec);
	}
	if (auto map = ge->GetTask<JewelryMap::Object>("本編", "宝石マップ"))
	{
		ML::Vec2 preVec{
			this->GetPos().x-ge->camera2D.x+(cos(this->UpdateDrillAngle()) * 16.f),
				this->GetPos().y-ge->camera2D.y +(sin(this->UpdateDrillAngle()) * 16.f)
		};
		map->Search(preVec);
	}
	
}



void Drill::SearchBrocks(ML::Vec2 pos)
{
	auto map = ge->GetTask < Map::Object >(Map::defGroupName, Map::defName);
	if (nullptr == map) { return; }

}

ML::Vec2 Drill::ChangeBrockPos()
{
	return ML::Vec2{
		(this->GetPos() + this->DrillAngleVec()*16.f) / 16.f
	};
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
		if (true == map->CheckHit(hit)) 
		{
			//移動をキャンセル
			SetPosX(preX);
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
		if (true == map->CheckHit(hit)) 
		{
			//移動をキャンセル
			this->SetPosY(preY);
			break;
		}

	}

}

void Drill::InitDurability(int num)
{
	this->durability = num;
	this->maxDurability = num;
}

void Drill::SetDurability(int num)
{
	this->durability = num;
}

int Drill::GetDurability()
{
	return this->durability;
}

int Drill::GetMaxDurability()
{
	return this->maxDurability;
}

float Drill::GetNowDurabilityPercent()
{
	return this->durability / this->maxDurability;
}

void Drill::ResetDurability()
{
	this->durability = this->maxDurability;
	this->srcX = 0;
}

ML::Vec2 Drill::GetAttackPos()
{
	return this->GetPos() + ML::Vec2{ (cos(GetNowAngle()) * 2.f), (sin(GetNowAngle()) * 2.f) };
}

void Drill::setAnim()
{

	this->animManager_->SetDefaultAnimId((int)Mode::Non);
	
	ML::Color defColor{ 1,1,1,1 };
	ML::Box2D drawSize {- 4, -4, 8, 8};
	int srcX = 0;
	AnimInfo animTable[] =
	{
		{drawSize,ML::Box2D{srcX,0,0,0},defColor,1,1},
		{drawSize,ML::Box2D{srcX,0,64,64},defColor,1,1},
	};
	this->animManager_->AddAnim((int)Mode::Non, animTable[0]);
	this->animManager_->AddAnim((int)Mode::Normal, animTable[1]);
	this->animManager_->AddAnim((int)Mode::Drill, animTable[1]);

}

void Drill::SetAnimMove(ML::Vec2 move_)
{
	this->animMove = move_;
}

ML::Vec2 Drill::GetAnimMove()
{
	return this->animMove;
}