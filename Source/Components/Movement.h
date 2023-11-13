#pragma once

#include "../../Component.h"
#include "../../GameEngine_Ver3_83.h"

class Movement : public Component
{
private:
	ML::Vec2	direction;	//�ړ�����
	ML::Vec2	velocity;	//�ړ��x�N�g��
	float		speed;		//�ړ����x

	void SetMoveFromKey(int key_);
public:
	enum class Dir_2
	{
		Holizontal,
		Vertical,
	};
	Movement(class Actor* owner);
	Movement(class Actor* owner,float speed_);

	float GetSpeed() const;
	ML::Vec2 GetDirection() const;
	ML::Vec2 GetVelocity() const;

	void SetSpeed(float speed_);
	void SetDirection(ML::Vec2 direction_);
	void LStickInputToMoveVelocity(XI::GamePad::SP controller_);

	void MoveOn(ML::Vec2& pos);
};