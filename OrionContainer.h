#pragma once
#include "Actor.h"
class OrionContainer : public Actor
{
	const float movementAmount_;
	float limitRange_;
public:
	OrionContainer(const float moveAmount);
	virtual ~OrionContainer() {};

	void HitPlayer();

	void Move();
};

