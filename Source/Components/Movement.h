#pragma once

#include "../../Component.h"
#include "../../GameEngine_Ver3_83.h"

class Movement : public Component
{
private:	
	ML::Vec2		velocity_;					//���x

	float			initSpeed_;					//�����̑���
	float			maxSpeed_;					//�ő�̑���
	float			nowSpeed_;					//���݂̑���
	float			stopSpeed_;					//��~���鑬��

	float			acceleration_;				//�����x
	ML::Percentage	decelerationRate_;			//������

	ML::Vec2		direction_;					//�ړ�����

	bool			considerationCollition_;	//�����蔻����l��

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
	
	//�����̃Z�b�^�[
	void SetInitSpeed(const float initSpeed);
	void SetMaxSpeed(const float maxSpeed);
	void SetStopSpeed(const float stopSpeed);
	void SetSpeed(const float initSpeed, const float maxSpeed, const float stopSpeed);

	//���������̃Z�b�^�[
	void SetDecelerationRate(const ML::Percentage& decelerationRate);
	void SetAcceleration(const float acceleration);

	//�����̃Z�b�^�[
	void SetDirection(const ML::Vec2& direction);

	void SetConsiderationCollition(const bool consideration);
	bool IsConsiderationCollition() const;

	//�R���g���[���̃X�e�B�b�N���͂���ړ�����
	void LStickInputToMove(const XI::GamePad::SP& controller);

	void Accel();
	void Decel();
	void Stop();
private:
	static ML::Vec2 CalcVelocity(const float speed, ML::Vec2 direction);
	void CalcVelocity();
	void MoveOn();
};