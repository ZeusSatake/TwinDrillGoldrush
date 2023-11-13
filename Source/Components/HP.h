#pragma once
#include "../../Component.h"
class HP : public Component
{
private:
	int now;
	int max;
	bool isAlive;

	//������ԍX�V
	void UpdateIsAlive();
public:
	enum class MaxLifeSetMode {
		MaxHeal,
		DifferenceHeal,
		NonHeal
	};

	HP(class Actor* owner);
	HP(class Actor* owner,int max_);

	//���݃��C�t
	void TakeDamage(const int damage_);
	void TakeHeal(const int heal_);
	int GetNowHP() const;

	//�ő僉�C�t
	void SetMaxHP(const int max_, MaxLifeSetMode setMode);
	void AddMaxHP(const int add, MaxLifeSetMode setMode);
	void SubMaxHP(const int sub);
	void Initialize(const int max_);
	//void SetMaxLife(const int& max_, MaxLifeSetMode interpolationMode);
	int GetMaxHP() const;

	//�����Ă���
	bool IsAlive() const;
};