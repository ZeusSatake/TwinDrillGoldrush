#pragma once
#include "Component.h"
//’lŽD‚Å‚²‚´‚é

class PriceTagComponent : public Component
{
public:
	PriceTagComponent(class GameObject* owner);
	virtual ~PriceTagComponent() {};
	virtual void Update() override;
};

