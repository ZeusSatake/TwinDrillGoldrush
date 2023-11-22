#pragma once

#include "../../Component.h"
#include "../../myLib.h"
#include "../../Character.h"
class AIMoveComponent : public Movement
{
public:
	ML::Vec2 moveVec_;
public:
	AIMoveComponent(class Character* owner,ML::Vec2 moveVec,float moveSpeed);
	AIMoveComponent(class Character* owner);
	virtual ~AIMoveComponent() {};

	void Update() override;

	void MoveTo(class Actor* target);

	void Patroll(class Actor* target);

	void Jump();

	void KnockBack();
};

