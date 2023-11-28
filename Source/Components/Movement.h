#pragma once

#include "../../Component.h"
#include "../../GameEngine_Ver3_83.h"

class Movement : public Component
{
private:	
	ML::Vec2		velocity_;					//速度

	float			initSpeed_;					//初期の速さ
	float			maxSpeed_;					//最大の速さ
	float			nowSpeed_;					//現在の速さ
	float			stopSpeed_;					//停止する速さ

	float			acceleration_;				//加速度
	ML::Percentage	decelerationRate_;			//減速率

	ML::Vec2		direction_;					//移動方向

	bool			considerationCollition_;	//当たり判定を考慮

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
	
	//速さのセッター
	void SetInitSpeed(const float initSpeed);
	void SetMaxSpeed(const float maxSpeed);
	void SetStopSpeed(const float stopSpeed);
	void SetSpeed(const float initSpeed, const float maxSpeed, const float stopSpeed);

	//加速減速のセッター
	void SetDecelerationRate(const ML::Percentage& decelerationRate);
	void SetAcceleration(const float acceleration);

	//向きのセッター
	void SetDirection(const ML::Vec2& direction);

	void SetConsiderationCollition(const bool consideration);
	bool IsConsiderationCollition() const;

	//コントローラのスティック入力から移動する
	void LStickInputToMove(const XI::GamePad::SP& controller);

	void Accel();
	void Decel();
	void Stop();
private:
	static ML::Vec2 CalcVelocity(const float speed, ML::Vec2 direction);
	void CalcVelocity();
	void MoveOn();
};