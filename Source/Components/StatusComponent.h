#pragma once
#include "../../Component.h"
#include "../Components/HP.h"
class StatusComponent : public Component
{
	
public:
	StatusComponent(class Actor* owner);
	virtual ~StatusComponent() {};

	HP hp_;
	int attackVal_;
	int defenseVal_;
	float speedVal_;

	HP GetHP() const;

	int GetAttackVal() const;

	void SetAttackVal(const int attack);

	int GetDefenseVal() const;

	void SetDefenseVal(const int defense);

	float GetSpeedVal() const;

	void SetSpeedVal(const float speed);
};

