#pragma once
#include "../../Component.h"
class HP : public Component
{
private:
	int now;
	int max;
	bool isAlive;

	//生存状態更新
	void UpdateIsAlive();
public:
	enum class MaxLifeSetMode {
		MaxHeal,
		DifferenceHeal,
		NonHeal
	};

	HP(class Actor* owner);
	HP(class Actor* owner,int max_);

	//現在ライフ
	void TakeDamage(const int damage_);
	void TakeHeal(const int heal_);
	int GetNowHP() const;

	//最大ライフ
	void SetMaxHP(const int max_, MaxLifeSetMode setMode);
	void AddMaxHP(const int add, MaxLifeSetMode setMode);
	void SubMaxHP(const int sub);
	void Initialize(const int max_);
	//void SetMaxLife(const int& max_, MaxLifeSetMode interpolationMode);
	int GetMaxHP() const;

	//生きている
	bool IsAlive() const;
};