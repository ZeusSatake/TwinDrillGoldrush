#pragma once
#include "Component.h"
//���z�ł�����

class WalletComponent : public Component
{
public:
	WalletComponent(class GameObject* owner);
	virtual ~WalletComponent() {};
	virtual void Update() override;
};

