#pragma once
#include "Character.h"
#include "Source/Components/AIMoveComponent.h"

class NPC : public Character
{
	float fov_;
	float distance_;
	Actor* target_;
protected:
	shared_ptr<AIMoveComponent> AIMove_;
public:
	NPC();
	virtual ~NPC() {};

	//‘O–Ê‚Ì”»’è
	bool CheckFront() const;
	bool CheckFrontFoot() const;

	float GetFov() const;
	void SetFov(const float fov);

	float GetDistance() const;
	void SetDistance(const float distance);

	Actor* GetTarget() const;
	void SetTarget(Actor* target);
};

