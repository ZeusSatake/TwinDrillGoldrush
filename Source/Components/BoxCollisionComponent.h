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

	//ÚG‚µ‚½‚ç‚»‚ÌƒNƒ‰ƒX‚ğ•Ô‚·‚µ‚Ä‚¢‚È‚¯‚ê‚Înullptr‚ğ•Ô‚·(Œ»ó‘Šè‘¤‚©‚çŒÄ‚Ôê‡‚É‚µ‚©g‚¦‚È‚¢)
	template<class T>
	T* CheckHitObject(const ML::Box2D& hit) const;

	ML::Box2D getHitBase() const;
	void setHitBase(const ML::Box2D& hitBase);
};

