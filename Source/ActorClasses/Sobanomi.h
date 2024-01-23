#pragma once
#include "../../Actor.h"
#include "../EnemyClasses/LadyNegishi.h"
class Sobanomi : public Actor
{
	const float fallSpeed_;
	const int damage_;
	LadyNegishi* owner_;
	
public:
	Sobanomi();
	virtual ~Sobanomi() {};

	void Update();

	void SetOwner(LadyNegishi* owner) { owner_ = owner; }
};

