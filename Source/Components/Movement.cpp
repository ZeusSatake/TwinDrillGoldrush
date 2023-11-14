#include "Movement.h"
#include "../../Actor.h"

Movement::Movement(Actor* owner)
	: Component(owner),
	direction_(0.0f, 0.0f),
	velocity_(0.0f, 0.0f),
	acceleration_(0.0f),
	decelerationRate_(),
	maxSpeed_(0.0f),
	stopSpeed_(0.0f),
	initSpeed_(0.0f),
	nowSpeed_(initSpeed_)
{
}
Movement::Movement(class Actor* owner, float initSpeed)
	: Component(owner),
	direction_(0.0f, 0.0f),
	velocity_(0.0f, 0.0f),
	acceleration_(0.0f),
	decelerationRate_(),
	maxSpeed_(0.0f),
	stopSpeed_(0.0f),
	initSpeed_(initSpeed),
	nowSpeed_(initSpeed_)
{
}

float Movement::GetSpeed() const
{
	return nowSpeed_;
}
ML::Vec2 Movement::GetDirection() const
{
	return direction_;
}
ML::Vec2 Movement::GetVelocity() const
{
	return velocity_;
}

void Movement::SetSpeed(const float speed)
{
	if (nowSpeed_ == speed)
		return;
	nowSpeed_ = speed;
	CalcVelocity();
}
void Movement::SetDirection(const ML::Vec2& direction)
{
	if (direction_ == direction)
		return;
	direction_ = direction;
	CalcVelocity();
}
void Movement::SetDecelerationRate(const ML::Percentage& decelerationRate)
{
	decelerationRate_ = decelerationRate;
}

void Movement::LStickInputToMoveVelocity(XI::GamePad::SP controller)
{
	auto inp = controller->GetState();
	int key = 0;
	if (inp.LStick.BU.on)
		key += 1;
	if (inp.LStick.BD.on)
		key += 2;
	if (inp.LStick.BL.on)
		key += 4;
	if (inp.LStick.BR.on)
		key += 8;

	SetMoveFromKey(key);
}

void Movement::SetMoveFromKey(int key)
{
	//0, 1, 2,  3,  4,  5,  6, 7, 8, 9,10,
	int table[] = { -1,90,270,-1,180,135,225,-1, 0,45,315 };

	ML::Vec2 est;
	//0‚Í“ÁŽêðŒ
	if (key == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float) cos(table[key] * D3DX_PI / 180.f);
		est.y = (float)-sin(table[key] * D3DX_PI / 180.f);
	}

	direction_ = est;
	CalcVelocity();
}
ML::Vec2 Movement::CalcVelocity(float speed, ML::Vec2 direction)
{
	return direction * speed;
}

void Movement::CalcVelocity()
{
	velocity_ = CalcVelocity(nowSpeed_, direction_);
}

void Movement::Accel()
{
	nowSpeed_ += acceleration_ / REFRESHRATE;
	if (nowSpeed_ > maxSpeed_)
		nowSpeed_ = maxSpeed_;

	SetSpeed(nowSpeed_);
	owner_->pos_ += velocity_;
}

void Movement::Decel()
{
	nowSpeed_ *= decelerationRate_.Get() / REFRESHRATE;
	if (nowSpeed_ <= stopSpeed_)
		nowSpeed_ = 0.0f;

	SetSpeed(nowSpeed_);
	owner_->pos_ += velocity_;
}