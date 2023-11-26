//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "Actor.h"
#include  "MyPG.h"

Actor::Actor()
	:GameObject()
{
	AddComponent(box_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
}

//��ʊO�ɏo�Ă�����ture��Ԃ�
bool Actor::OutOfScreen() const
{
	ML::Box2D halfHitBase;
	halfHitBase.w = this->box_->getHitBase().w * 0.5f;
	halfHitBase.h = this->box_->getHitBase().h * 0.5f;

	ML::Rect bodyEdge
	{
		int(this->pos_.x - halfHitBase.w),//��
		int(this->pos_.y - halfHitBase.h),//��
		int(this->pos_.x + halfHitBase.w),//�E
		int(this->pos_.y + halfHitBase.h) //��
	};

	//��ʊO�Ȃ�true
	//��
	if (bodyEdge.bottom < 0.0f)
		return true;

	//��
	if (bodyEdge.right < 0.0f)
		return true;

	//�E
	if (bodyEdge.left > ge->gameScreenWidth)
		return true;

	//��
	if (bodyEdge.top > (int)ge->screenHeight)
		return true;


	//��ʊO�ɏo�Ă��Ȃ�
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

