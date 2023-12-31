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

	//接触したらそのクラスを返すしていなければnullptrを返す(現状相手側から呼ぶ場合にしか使えない)
	template<class T>
	T* CheckHitObject(const ML::Box2D& hit) const;

	ML::Box2D getHitBase() const;
	void setHitBase(const ML::Box2D& hitBase);
};

