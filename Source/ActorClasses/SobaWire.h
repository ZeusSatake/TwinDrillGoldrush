#pragma once
#include "../../Actor.h"
#include "../EnemyClasses/LadyNegishi.h"
class SobaWire : public Actor
{
	LadyNegishi* owner_;
	vector<ML::Vec2> weakPointPos_;
protected:
	vector<shared_ptr<BoxCollisionComponent>> weakPoint_;
public:
	SobaWire();
	virtual ~SobaWire() {};

	void Update();

	void SetWeakPointPos(const ML::Vec2 pos, const int index) { weakPointPos_[index] = pos; }
	vector<ML::Vec2> GetWeakPointPos() { return weakPointPos_; }
	void SetOwner(LadyNegishi* owner) { owner_ = owner; }
};

