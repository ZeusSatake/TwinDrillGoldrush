#include "Component.h"
#include "GameObject.h"

//�R���X�g���N�^
Component::Component(class Actor* owner)
{
	owner_ = owner;
}
void Component::Update()
{
}