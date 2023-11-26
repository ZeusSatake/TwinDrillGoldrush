//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "Actor.h"
#include  "MyPG.h"

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

ML::Vec2 Actor::GetMoveVec() const
{
	return moveVec_;
}

void Actor::SetMoveVec(const ML::Vec2 vec)
{
	moveVec_ = vec;
}

float Actor::GetGravity() const
{
	return gravity_;
}

void Actor::SetGravity(const float gravity)
{
	gravity_ = gravity;
}

float Actor::GetMaxFallSpeed() const
{
	return maxfallSpeed_;
}

void Actor::SetMaxFallSpeed(const float maxFallSpeed)
{
	maxfallSpeed_ = maxFallSpeed;
}

float Actor::GetJumpPower() const
{
	return jumpPower_;
}

void Actor::SetJumpPower(const float jumpPower)
{
	jumpPower_ = jumpPower;
}

