#include "Character.h"

Character::Character()
{
	//コンポーネントの追加
	AddComponent(hp = shared_ptr<HP>(new HP(this)));
	AddComponent(movement = shared_ptr<Movement>(new Movement(this)));
}