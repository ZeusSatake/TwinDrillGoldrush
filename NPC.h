#pragma once
#include "Character.h"
#include "Source/Components/AIMoveComponent.h"
#include "Source/Components/AIComponent.h"
class NPC : public Character
{
protected:
	class shared_ptr<AIComponent> AI_;
	
public:
	NPC();
	virtual ~NPC() {};
};

