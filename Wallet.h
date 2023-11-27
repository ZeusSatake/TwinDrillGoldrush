#pragma once
#include "Component.h"
//���z�ł�����

class PriceTagComponent;

class WalletComponent : public Component
{
	int balance_;
public:
	WalletComponent(class GameObject* owner);
	virtual ~WalletComponent() {};
	virtual void Update() override;

	bool Payment(const int price);
	bool Recieve(const int price);

	int GetBalance() const;
};