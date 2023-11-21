#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�[�p���N���X
//-----------------------------------------------------------------------------
#include "Actor.h"
#include "source/Components/HP.h"
#include "Source/Components/Animation/AnimManager.h"
#include "Source/Components/Movement.h"
class Character : public Actor
{
	std::string personalName_;
public:
	typedef shared_ptr<Character> SP;
	typedef weak_ptr<Character> WP;
protected:
	//-----------------------------------------------------------------------------
	//�R���|�[�l���g
	class shared_ptr<HP> hp_;
	class shared_ptr<Movement> movement_;
	class shared_ptr<AnimManager> animManager_;
	//-----------------------------------------------------------------------------
public:
	Character();
	virtual  ~Character() {}

	//�v�l
	virtual void Think();
	//����
	virtual void Move();

	shared_ptr<HP> GetHP() const;
	shared_ptr<Movement> GetMovement() const;

	std::string GetPersonalName()const;
	void SetPersonalName(const std::string name);

};

