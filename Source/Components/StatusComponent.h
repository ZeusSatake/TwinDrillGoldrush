#pragma once
#include "../../Component.h"
class StatusComponent : public Component
{
	
public:
	StatusComponent(class Actor* owner);
	virtual ~StatusComponent() {};

	int defenceVal;
	int speedVal;

};

