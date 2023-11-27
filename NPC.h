#pragma once
#include "Character.h"
#include "Source/Components/AIMoveComponent.h"

#include "Source/Components/AIComponent.h"
class NPC : public Character
{
protected:
	
public:
	NPC();
	virtual ~NPC() {};

	//‘O–Ê‚Ì”»’è
	bool CheckFront() const;
	bool CheckFrontFoot() const;
};

