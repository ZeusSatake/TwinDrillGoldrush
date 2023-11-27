//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "Actor.h"
#include  "MyPG.h"
#include "Source/Scene/Task_Map.h"

Actor::Actor()
	:GameObject()
{
	AddComponent(box_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
}

//画面外に出ていたらtureを返す
bool Actor::OutOfScreen() const
{
	ML::Box2D halfHitBase;
	halfHitBase.w = this->box_->getHitBase().w * 0.5f;
	halfHitBase.h = this->box_->getHitBase().h * 0.5f;

	ML::Rect bodyEdge
	{
		int(this->pos_.x - halfHitBase.w),//上
		int(this->pos_.y - halfHitBase.h),//左
		int(this->pos_.x + halfHitBase.w),//右
		int(this->pos_.y + halfHitBase.h) //下
	};

	//画面外ならtrue
	//上
	if (bodyEdge.bottom < 0.0f)
		return true;

	//左
	if (bodyEdge.right < 0.0f)
		return true;

	//右
	if (bodyEdge.left > ge->gameScreenWidth)
		return true;

	//下
	if (bodyEdge.top > (int)ge->screenHeight)
		return true;


	//画面外に出ていない
	return false;


}



bool Actor::CheckHit(const ML::Box2D& target) const
{
	ML::Box2D me = this->box_->getHitBase().OffsetCopy(this->pos_);
	return me.Hit(target);
}

void Actor::CheckMove(ML::Vec2& est)
{
	//マップが存在するか調べてからアクセス
	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) return;//マップが無ければ判定しない(出来ない）

	//横軸に対する移動
	while (est.x != 0) 
	{
		float  preX = this->pos_.x;
		if (est.x >= 1) 
		{ 
			this->pos_.x += 1;		
			est.x -= 1; 
		}
		else if (est.x <= -1) 
		{ 
			this->pos_.x -= 1;		
			est.x += 1; 
		}
		else 
		{ 
			this->pos_.x += est.x;		
			est.x = 0; 
		}
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) 
		{
			this->pos_.x = preX;		//移動をキャンセル
			break;
		}
	}
	//縦軸に対する移動
	while (est.y != 0) 
	{
		float  preY = this->pos_.y;
		if (est.y >= 1) 
		{ 
			this->pos_.y += 1;
			est.y -= 1; 
		}
		else if (est.y <= -1) 
		{ 
			this->pos_.y -= 1;		
			est.y += 1; 
		}
		else 
		{ 
			this->pos_.y += est.y;		
			est.y = 0; 
		}
		
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) 
		{
			this->pos_.y = preY;		//移動をキャンセル
			break;
		}
	}
}



