#pragma once
#include "Component.h"
//財布でござる

class WalletComponent : public Component
{
public:
	WalletComponent(class GameObject* owner);
	virtual ~WalletComponent() {};
	virtual void Update() override;
};

