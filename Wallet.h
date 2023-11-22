#pragma once
#include "Component.h"
//ç‡ïzÇ≈Ç≤Ç¥ÇÈ

class WalletComponent : public Component
{
public:
	WalletComponent(class GameObject* owner);
	virtual ~WalletComponent() {};
	virtual void Update() override;
};

