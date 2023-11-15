#pragma once

#include "../../Component.h"
#include "../../GameEngine_Ver3_83.h"

class Movement : public Component
{
private:
	ML::Vec2		direction_;			//移動方向
	ML::Vec2		velocity_;			//速度
	float			acceleration_;		//加速度
	ML::Percentage	decelerationRate_;  //減速率
	float			initSpeed_;			//初期の速さ
	float			maxSpeed_;			//最大の速さ
	float			nowSpeed_;			//現在の速さ
	float			stopSpeed_;			//停止する速さ

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