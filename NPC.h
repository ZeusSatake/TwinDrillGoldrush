#pragma once
#include "Character.h"
#include "Source/Components/AIMoveComponent.h"
class NPC : public Character
{
	class shared_ptr<AIMoveComponent> AI_;
public:
	NPC();
	virtual ~NPC() {};


};

