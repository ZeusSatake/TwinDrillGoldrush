#pragma once
#include "../../Component.h"
#include "../Components/HP.h"

class StatusComponent : public Component
{
public:
	StatusComponent(Actor* owner);
	virtual ~StatusComponent() {};
	struct Speed
	{
	private:
		float now_;
		float max_;
		float fallSpeed_;
	public:
		void Initialize(const float now, const float max, const float fallSpeed);
		float GetNow() const;
		float GetMax() const;
		float GetFallSpeed() const;
		void AddSpeed(const float add);
		void SetMax(const float max);
		void SetFallSpeed(const float fallSpeed);
	};
	struct Attack
	{
	private:
		int now_;
		int max_;
	public:
		void Initialize(const int now, const int max);
		int GetNow() const;
		int GetMax() const;
		void AddAttack(const int add);
		void SetMax(const int max);
	};
	struct Defence
	{
	private:
		int now_;
		int max_;
	public:
		void Initialize(const int now, const int max);
		int GetNow() const;
		int GetMax() const;
		void AddDefence(const int add);
		void SetMax(const int max);
	};
	struct Health
	{
	private:
		int now_;
		int max_;
		bool isAlive_;
	public:
		//������ԍX�V
		void UpdateIsAlive();
	public:
		enum class MaxLifeSetMode 
		{
			MaxHeal,
			DifferenceHeal,
			NonHeal
		};
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
		
	/*StatusComponent(Actor* owner,Speed& spd,Attack& atk,Defence& def,Health& hp)
		: Component(owner)
		, speed(spd)
		, attack(atk)
		, defence(def)
		, HP(hp)
	{
	}*/

	Speed speed;	
	Attack attack;
	Defence defence;
	Health HP;
};

