#include "Component.h"
#include "GameObject.h"

//コンストラクタ
Component::Component(class Actor* owner)
{
	owner_ = owner;
}
void Component::Update()
{
}