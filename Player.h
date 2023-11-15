#pragma once
#include "Character.h"
#include "Source/Components/ControllerInputComponent.h"
class Player :public Character
{
	class std::shared_ptr<ControllerInputComponent> Controller;
public:
	Player();
	virtual ~Player();
};