#pragma once
#include "Component.h"
//�l�D�ł�����

class PriceTagComponent : public Component
{
public:
	PriceTagComponent(class GameObject* owner);
	virtual ~PriceTagComponent() {};
	virtual void Update() override;
};

