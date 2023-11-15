#pragma once
#include "Component.h"
class AIComponent : public Component
{

public:
	AIComponent(class Character* owner);
	virtual ~AIComponent();
};

