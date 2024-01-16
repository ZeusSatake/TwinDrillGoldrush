#pragma once
#include "../../Actor.h"
class OrionContainer : public Actor
{
	const float movementAmount_;
	float limitRange_;
protected:
	shared_ptr<class BoxCollisionComponent> fowardCollision_;
	shared_ptr<class BoxCollisionComponent> topCollision_;
public:
	OrionContainer();
	virtual ~OrionContainer() {};

	void HitPlayer();

	virtual void Move();

	void HitFoward();

	void HitTop();
};

