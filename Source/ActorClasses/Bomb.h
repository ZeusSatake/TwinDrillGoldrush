#pragma once
#include "../../Actor.h"
#include "../EnemyClasses/LadyKiyohara.h"
class Bomb : public Actor
{
	bool isGiveDamage_;
	bool isReflected_;
	const int damage_;
	const float movementAmount_;
	LadyKiyohara* owner_;
	shared_ptr<class TimerComponent> bombCnt_;
protected:
	shared_ptr<class BoxCollisionComponent> blastBox_;
public:
	Bomb();
	virtual ~Bomb() {};

	void Move();

	void ChangeReflect();

	void ChangeExplosion();

	void SetOwner(LadyKiyohara* owner) { owner_ = owner; }

	enum BombState
	{
		Non,
		Fall,
		Reflect,
		Explosion,
	};

private:
	BombState bombState_;
public:
	BombState GetBombState() const { return bombState_; }
};

