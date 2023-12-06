#pragma once

#include "../../Component.h"
#include "../../myLib.h"
#include "../../Character.h"
#include "../../Player.h"
class AIMoveComponent : public Component
{
public:
	shared_ptr<Player> pl;
public:
	AIMoveComponent(class Character* owner);
	virtual ~AIMoveComponent() {};

	void Update() override;

	void MoveTo(class Actor* target);

	void Patroll();

	void Jump();

	void KnockBack();
};

