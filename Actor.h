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
public:
	typedef shared_ptr<Actor> SP;
	typedef weak_ptr<Actor>	WP;
public:
	//�ύX������������������������������������������������������
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


public:
	bool OutOfScreen() const;


	//�����蔻��
	bool CheckHit(const ML::Box2D& hit) const;


};