#pragma once
//-----------------------------------------------------------------------------
//キャラクター用基底クラス
//-----------------------------------------------------------------------------
#include "Actor.h"
#include "source/Components/HP.h"
#include "Source/Components/Animation/AnimManager.h"
#include "Source/Components/Movement.h"
#include "Source/Components/TimerComponent.h"
class Character : public Actor
{
	std::string personalName_;
public:
	typedef shared_ptr<Character> SP;
	typedef weak_ptr<Character> WP;
protected:
	//-----------------------------------------------------------------------------
	//コンポーネント
	class shared_ptr<HP> hp_;
	class shared_ptr<AnimManager> animManager_;
	class shared_ptr<TimerComponent> unHitTimer_;
	//-----------------------------------------------------------------------------
public:
	Character();
	virtual  ~Character() {}

	//思考
	virtual void Think();
	//動作
	virtual void Move();
	//足元判定
	bool CheckFoot() const;

	shared_ptr<HP> GetHP() const;

	std::string GetPersonalName()const;
	void SetPersonalName(const std::string name);


};

