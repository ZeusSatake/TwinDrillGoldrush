#pragma once
class Component
{
public:
	Component(class Actor* owner);
	virtual ~Component() {}
	//���t���[���X�V(�g���ꍇ�e�^�X�N��Update�ŌĂԂ悤��)
	virtual void Update();
protected:
	class Actor* owner_;
};

