#include "BossLady.h"

class LadyKumagai : public BossLady
{
	const int standCnt_;
	const int attackCnt_;

	float midRange_;
public:
	LadyKumagai();
	virtual ~LadyKumagai() {};
protected:
	virtual void Think();
	virtual void Move();
	virtual void UpDateApproach() override;
	virtual void UpDateJump() override;
	virtual void UpDateFall() override;
	virtual void UpDateAttackStand() override;
	virtual void UpDateAttack() override;
	virtual void UpDateDamage() override;
	virtual void UpDateDead() override;

	void CreateContainer();
};