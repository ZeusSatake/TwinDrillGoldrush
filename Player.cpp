#include "Player.h"
Player::Player()
{
	AddComponent(std::shared_ptr<ControllerInputComponent>(new ControllerInputComponent(this)));
}