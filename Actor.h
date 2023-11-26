#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�Q�[�����I�u�W�F�N�g���N���X
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include "Source/Components/BoxCollisionComponent.h"
#include "Source/Components/HP.h"


class Actor : public GameObject
{
	ML::Vec2 moveVec_;
	float gravity_;
	float maxfallSpeed_;
	float jumpPower_;
public:
	typedef shared_ptr<Actor> SP;
	typedef weak_ptr<Actor>	WP;
public:
	ML::Vec2    pos_;		//�L�����N�^�ʒu
	int			moveCnt_;	//�s���J�E���^
	//�����i2D���_�j
	float angle_;

	enum class Angle_LR
	{
		Left,Right
	};
	Angle_LR angle_LR;
protected:
	//�R���|�[�l���g
	shared_ptr<BoxCollisionComponent> box_;

public:
	Actor();
	virtual  ~Actor() {}
	ML::Vec2 GetMoveVec() const;
	void SetMoveVec(const ML::Vec2 vec);

	float GetGravity() const;
	void SetGravity(const float gravity);

	float GetMaxFallSpeed() const;
	void SetMaxFallSpeed(const float maxFallSpeed);

	float GetJumpPower() const;
	void SetJumpPower(const float jumpPower);

public:
	bool OutOfScreen() const;


	//�����蔻��
	bool CheckHit(const ML::Box2D& hit) const;


};