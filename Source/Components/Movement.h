#pragma once

#include "../../Component.h"
#include "../../GameEngine_Ver3_83.h"

class Movement : public Component
{
private:
	ML::Vec2		direction_;			//�ړ�����
	ML::Vec2		velocity_;			//���x
	float			acceleration_;		//�����x
	ML::Percentage	decelerationRate_;  //������
	float			initSpeed_;			//�����̑���
	float			maxSpeed_;			//�ő�̑���
	float			nowSpeed_;			//���݂̑���
	float			stopSpeed_;			//��~���鑬��

	void SetMoveFromKey(int key);
public:
	enum class Dir_2
	{
		Holizontal,
		Vertical,
	};
	Movement(class Actor* owner);
	Movement(class Actor* owner,float speed);

	float GetSpeed() const;
	ML::Vec2 GetDirection() const;
	ML::Vec2 GetVelocity() const;

	void SetSpeed(const float speed);
	void SetDirection(const ML::Vec2& direction);
	void SetDecelerationRate(const ML::Percentage& decelerationRate);
	void LStickInputToMoveVelocity(XI::GamePad::SP controller);

	void Accel();
	void Decel();
private:
	static ML::Vec2 CalcVelocity(float speed, ML::Vec2 direction);
	void CalcVelocity();
};