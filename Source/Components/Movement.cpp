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
	nowSpeed_(initSpeed_),
	considerationCollition_(false)
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
	nowSpeed_(initSpeed_),
	considerationCollition_(false)
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

void CheckSpeedSetError(const float speed, const char const* msg)
{
	if (speed < 0.0f)
		assert(!msg);
}
void Movement::SetInitSpeed(const float initSpeed)
{
	CheckSpeedSetError(initSpeed, "設定しようとしている初期の早さが0未満です。0以上にしてください。");

	initSpeed_ = initSpeed;
}
void Movement::SetMaxSpeed(const float maxSpeed)
{
	CheckSpeedSetError(maxSpeed, "設定しようとしている最大の早さが0未満です。0以上にしてください。");
	
	if (maxSpeed_ == maxSpeed)
		return;

	maxSpeed_ = maxSpeed;
	CalcVelocity();
}
void Movement::SetStopSpeed(const float stopSpeed)
{
	CheckSpeedSetError(stopSpeed, "設定しようとしている停止する早さが0未満です。0以上にしてください。");

	if (stopSpeed_ == stopSpeed)
		return;

	stopSpeed_ = stopSpeed;
	CalcVelocity();
}
void Movement::SetSpeed(const float initSpeed, const float maxSpeed, const float stopSpeed)
{
	SetInitSpeed(initSpeed);
	SetMaxSpeed(maxSpeed);
	SetStopSpeed(stopSpeed);
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
void Movement::SetAcceleration(const float acceleration)
{
	acceleration_ = acceleration;
}

void Movement::SetConsiderationCollition(const bool consideration)
{
	considerationCollition_ = consideration;
}
bool Movement::IsConsiderationCollition() const
{
	return considerationCollition_;
}

void Movement::LStickInputToMove(const XI::GamePad::SP& controller)
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
	//0は特殊条件
	if (key == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float)cos(table[key] * D3DX_PI / 180.f);
		est.y = (float)-sin(table[key] * D3DX_PI / 180.f);
	}

	direction_ = est;

	if (direction_.Length() != 0.0f)
		Accel();
	else
		Decel();
}
ML::Vec2 Movement::CalcVelocity(const float speed, ML::Vec2 direction)
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

	CalcVelocity();

	MoveOn();
}

void Movement::Decel()
{
	nowSpeed_ *= decelerationRate_.GetNormalizeValue() / REFRESHRATE;
	if (nowSpeed_ <= stopSpeed_)
		nowSpeed_ = 0.0f;

	CalcVelocity();
	MoveOn();
}

void Movement::Stop()
{
	nowSpeed_ = 0.0f;
	CalcVelocity();
}

void Movement::MoveOn()
{
	if (considerationCollition_)
		owner_->CheckMove(velocity_);
	else
		owner_->SetPos(owner_->GetPos() + velocity_);
}