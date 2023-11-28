#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�Q�[�����I�u�W�F�N�g���N���X
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include "Source/Components/BoxCollisionComponent.h"
#include "Source/Components/HP.h"
#include "Source/Components/Movement.h"

class Actor : public GameObject
{	
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
	Angle_LR angle_LR_;
protected:
	//�R���|�[�l���g
	shared_ptr<BoxCollisionComponent> box_;
	class shared_ptr<Movement> movement_;
	class shared_ptr<Movement> gravity_;

public:
	Actor();
	virtual  ~Actor() {}

public:
	bool OutOfScreen() const;


	//�����蔻��
	bool CheckHit(const ML::Box2D& hit) const;

	//�߂荞�܂Ȃ��ړ�
	void CheckMove(ML::Vec2& move);

	BoxCollisionComponent* GetBox() const;
	Movement* GetMovement() const;
	Movement* GetGravity() const;
};