#pragma once
#include "../../../Component.h"
#include <memory>
//ç‡ïzÇ≈Ç≤Ç¥ÇÈ

class PriceTagComponent;
namespace Save { class Object; }

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

	void RoadHaveMoney(const std::shared_ptr<Save::Object>& save);
};