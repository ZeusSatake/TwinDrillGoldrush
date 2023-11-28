#pragma once
#include "../Components/AIComponent.h"

class EnemyAIComponent : public AIComponent
{
public:
	EnemyAIComponent(class Enemy* owner);
	virtual ~EnemyAIComponent() {};

	virtual void Update() override;
	virtual void UpdatePatrol() override;
	virtual void UpdateApproach() override;
	virtual void UpdateJump() override;
	virtual void UpdateAttack() override;
	virtual void UpdateGuard() override;
	virtual void UpdateDodge() override;
	virtual void UpdateDamage() override;
	virtual void UpdateDead() override;

	virtual void Think() override;
	virtual void Move() override;
};