#pragma once
#include "EnemyAIComponent.h"
//#include "../../Debtor.h"
//債務者AIコンポーネント
class DebtorAIComponent : public EnemyAIComponent
{
public:
	DebtorAIComponent(class Debtor* owner);
	virtual ~DebtorAIComponent() {};

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

	bool UpdateState(AIState nowState);

	bool HitPlayer();

};

