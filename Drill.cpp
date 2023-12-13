#include <math.h>

#include "Drill.h"
#include "MyPG.h"
#include "Source/Scene/Task_Map.h"
#include "Source/Scene/Task_JewelryMap.h"
#include "Source/Components/Blocks/BlockManager.h"

Drill::Drill()
	:
	attackPoint(0),
	angle(0.0f),
	addAngle(0.0f),
	preAngle(0.0f),
	durability(0),
	Length(300.f),
	moveVec(ML::Vec2{0,0}),
	canRotate(true)
{
	AddComponent(controller_ = shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
	AddComponent(state_ = shared_ptr<StateComponent>(new StateComponent(this)));
}



void Drill::SetAngle(float angle)
{
	this->angle = angle;
}

void Drill::SetDrawPos(ML::Vec2 pos)
{
	this->drawPos = pos;
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

int Drill::GetAttackPoint()
{
	return this->attackPoint;
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
		fabsf(this->GetTargetPos().y*16 - pos.x)
	};
	float hypo = (diff.x * diff.x) + (diff.y * diff.y);
	if (this->Length*this->Length > hypo)
	{
		return true;
	}
	return false;
}

void Drill::Mining()
{
	if (auto map = ge->GetTask<Map::Object>("本編", "マップ"))
	{
		ML::Vec2 preVec{
			this->GetPos().x-ge->camera2D.x+(cos(this->UpdateDrillAngle()) * 16.f),
				this->GetPos().y-ge->camera2D.y +(sin(this->UpdateDrillAngle()) * 16.f)
		};
		map->Search(preVec);

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

//void Drill::SearchBrocks(ML::Vec2 pos_)
//{
//	//現在のマスを出さない限りはどうしようもない
//	ML::Vec2 pos = pos_;
//	//ML::Point brock{ (int)pos_.x / 16,(int)pos_.y / 16 };//これで現在いるマスが取れる
//	if (auto map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName)) {
//		if (!map->Search(pos))
//		{
//			if (!map->Search(ML::Vec2{ pos.x, pos.y - 16.f }))
//			{
//				if (!map->Search(ML::Vec2{ pos.x - 16.f,pos.y }))
//				{
//					if (!map->Search(ML::Vec2{ pos.x + 16.f,pos.y }))
//					{
//						return;
//					}
//				}
//			}
//		}
//	}
//}

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
		if (true == map->CheckHit(hit)) {
			if (this->LimitLength(this->plPos))
			{
				e_.x += 16.f;
			}
			else
			{				//移動をキャンセル
				SetPosX(preX);
				break;
			}
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
			preVec.y = preY;
			//移動をキャンセル
			break;
		}
	}
	//this->UpdateTargetPos(preVec);
}