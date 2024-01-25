#pragma once
#include "Lady.h"
class NormalLady : public Lady
{
	const int standCnt_;//�U�������J�E���^
	const int attackCnt_;//�U���J�E���^
	const float adjustRange_;//�U�����蒲��
	ML::Vec2 attackPos_;
protected:
	shared_ptr<BoxCollisionComponent> fanEdge_;
public:
	NormalLady();
	virtual ~NormalLady() { };

public:
	virtual void Think();
	virtual void Move();
	virtual void UpDatePatrol() override;
	virtual void UpDateApproach() override;
	virtual void UpDateJump() override;
	virtual void UpDateFall() override;
	virtual void UpDateAttackStand() override;
	virtual void UpDateAttack() override;
	virtual void UpDateGuard() override;
	virtual void UpDateDodge() override;
	virtual void UpDateDamage() override;
	virtual void UpDateDead() override;

	float GetAdjustRange() const;

	ML::Vec2 GetAttackPos() const { return attackPos_; }
};

