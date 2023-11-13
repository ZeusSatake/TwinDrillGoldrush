#pragma once
class Component
{
public:
	Component(class Actor* owner);
	virtual ~Component() {}
	//毎フレーム更新(使い場合各タスクのUpdateで呼ぶように)
	virtual void Update();
protected:
	class Actor* owner_;
};

