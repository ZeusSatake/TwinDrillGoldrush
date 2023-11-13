#include "Movement.h"

Movement::Movement(Actor* owner)
	: Component(owner),
	direction(0.0f, 0.0f),
	velocity(0.0f, 0.0f),
	speed(0.0f)
{
}
Movement::Movement(class Actor* owner,float speed_)
	: Component(owner),
	direction(0.0f, 0.0f),
	velocity(0.0f, 0.0f),
	speed(speed_)
{
}

float Movement::GetSpeed() const
{
	return speed;
}
ML::Vec2 Movement::GetDirection() const
{
	return direction;
}
ML::Vec2 Movement::GetVelocity() const
{
	return velocity;
}

void Movement::SetSpeed(float speed_)
{
	if (speed == speed_)
		return;
	speed = speed_;
	velocity = direction * speed;
}
void Movement::SetDirection(ML::Vec2 direction_)
{
	if (direction == direction_)
		return;
	direction = direction_;
	velocity = direction * speed;
}

void Movement::LStickInputToMoveVelocity(XI::GamePad::SP controller_)
{
	auto inp = controller_->GetState();
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

void Movement::SetMoveFromKey(int key_)
{
	//0, 1, 2,  3,  4,  5,  6, 7, 8, 9,10,
	int table[] = { -1,90,270,-1,180,135,225,-1, 0,45,315 };

	ML::Vec2 est;
	//0ÇÕì¡éÍèåè
	if (key_ == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float) cos(table[key_] * D3DX_PI / 180.f);
		est.y = (float)-sin(table[key_] * D3DX_PI / 180.f);
	}

	direction = est;
	velocity = direction * speed;
}

void Movement::MoveOn(ML::Vec2& pos)
{
	pos += velocity;
}