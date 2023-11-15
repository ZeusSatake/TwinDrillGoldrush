#pragma once
//-----------------------------------------------------------------------------
//タスク群の基底クラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "MyPG.h"
#include "Component.h"

class GameObject : public BTask
{
	typedef shared_ptr<GameObject> SP;
	typedef weak_ptr<GameObject> WP;

	

protected:
	std::vector<class shared_ptr<Component>> components_;

public:
	GameObject();
	virtual  ~GameObject() {}

	void AddComponent(class shared_ptr<Component> component);
	void RemoveComponent(class shared_ptr<Component> component);

	void UpdateComponents();
};

