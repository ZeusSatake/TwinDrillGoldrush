#include "Character.h"

Character::Character()
{
	//�R���|�[�l���g�̒ǉ�
	AddComponent(hp = shared_ptr<HP>(new HP(this)));
	AddComponent(movement = shared_ptr<Movement>(new Movement(this)));
}