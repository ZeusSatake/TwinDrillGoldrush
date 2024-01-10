#pragma once
#include "Actor.h"
class OrionContainer : public Actor
{
	const float movementAmount_;
	float limitRange_;
public:
	OrionContainer();
	virtual ~OrionContainer() {};

	void HitPlayer();

	virtual void Move();
};

