#include "Character.h"

Character::Character()
	:Actor()
{
	//ƒRƒ“ƒ|[ƒlƒ“ƒg‚Ì’Ç‰Á

	AddComponent(hp_ = shared_ptr<HP>(new HP(this)));
	AddComponent(movement_ = shared_ptr<Movement>(new Movement(this)));
	AddComponent(animManager_ = shared_ptr<AnimManager>(new AnimManager(this,0)));
}

void Character::Think()
{

}

void Character::Move()
{

}

shared_ptr<HP> Character::GetHP() const
{
	return hp_;
}

shared_ptr<Movement> Character::GetMovement() const
{
	return movement_;
}