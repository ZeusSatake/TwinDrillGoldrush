#pragma once
#include "../../Actor.h"
#include "../../Source/Components/TimerComponent.h"
#include "../EnemyClasses/BossLady.h"
class SobaPrison : public Actor
{
	bool countFlag_;
	const int damage_;
	BossLady* owner_;
protected:
	shared_ptr<TimerComponent> expandTimer_;
public:
	SobaPrison();
	virtual ~SobaPrison() {};

	void Move();

	enum State
	{
		Non,
		Notice,
		Expand,
	};

private:
	State state_;
public:
	State GetState() { return state_; }

	void SetOwner(BossLady* owner) { owner_ = owner; }
	TimerComponent* GetTimer() { return expandTimer_.get(); }
};

