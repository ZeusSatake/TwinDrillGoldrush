#include "BossLady.h"

class LadySatake : public BossLady
{
	const int standCnt_;
    const int attackCnt_;

	float midRange_;
public:
	LadySatake();
	virtual ~LadySatake() {};
protected:
	shared_ptr<BoxCollisionComponent> fanEdge_;

	virtual void Think();
	virtual void Move();
	virtual void UpDateApproach() override;
	virtual void UpDateJump() override;
	virtual void UpDateFall() override;
	virtual void UpDateAttackStand() override;
	virtual void UpDateAttack() override;
	virtual void UpDateTackle();
	//virtual void UpDateGuard() override;
	//virtual void UpDateDodge() override;
	virtual void UpDateDamage() override;
	virtual void UpDateDead() override;

	float GetMidRange() const;
	void SetMidRange(const float midRange);
};