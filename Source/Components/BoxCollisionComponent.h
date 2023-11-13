#pragma once
#include "../../Component.h"
#include "../../myLib.h"
#include "../../GameObject.h"
class BoxCollisionComponent : public Component
{
	ML::Box2D hitBase_;
public:
	BoxCollisionComponent(class Actor* owner);

	virtual ~BoxCollisionComponent() {};

	bool CheckHit(const ML::Box2D& hit) const;

	//�ڐG�����炻�̃N���X��Ԃ����Ă��Ȃ����nullptr��Ԃ�(���󑊎葤����Ăԏꍇ�ɂ����g���Ȃ�)
	template<class T>
	T* CheckHitObject(const ML::Box2D& hit) const;

	ML::Box2D getHitBase() const;
	void setHitBase(const ML::Box2D& hitBase);
};

