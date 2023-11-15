#pragma once
//-----------------------------------------------------------------------------
//キャラクター用基底クラス
//-----------------------------------------------------------------------------
#include "Actor.h"
#include "source/Components/HP.h"
#include "Source/Components/Animation/AnimManager.h"
#include "Source/Components/Movement.h"
class Character : public Actor
{
public:
	typedef shared_ptr<Character> SP;
	typedef weak_ptr<Character> WP;
protected:
	//-----------------------------------------------------------------------------
	//コンポーネント
	class shared_ptr<HP> hp;
	class shared_ptr<Movement> movement;
	class shared_ptr<AnimManager> animManager;
	//-----------------------------------------------------------------------------
public:
	Character();
	virtual  ~Character() {}
};

