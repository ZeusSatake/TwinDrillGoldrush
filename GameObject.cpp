#include "GameObject.h"

GameObject::GameObject()
	:BTask()
{

}
void GameObject::AddComponent(shared_ptr<Component> component)
{
	components_.push_back(component);
}
void GameObject::RemoveComponent(shared_ptr<Component> component)
{
	auto itr = std::find(components_.begin(), components_.end(), component);
	if (itr != components_.end())
	{
		components_.erase(itr);
	}
}
void GameObject::UpdateComponents()
{
	for (auto& component : components_)
	{
		component->Update();
	}
}