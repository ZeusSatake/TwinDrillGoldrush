#pragma once
#include "Component.h"
//値札でござる

class PriceTagComponent : public Component
{
public:
	PriceTagComponent(class GameObject* owner);
	virtual ~PriceTagComponent() {};
	virtual void Update() override;
};

