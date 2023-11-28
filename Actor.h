#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//ゲーム内オブジェクト基底クラス
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include "Source/Components/BoxCollisionComponent.h"
#include "Source/Components/HP.h"
#include "Source/Components/Movement.h"

class Actor : public GameObject
{	
public:
	typedef shared_ptr<Actor> SP;
	typedef weak_ptr<Actor>	WP;
public:
	ML::Vec2    pos_;		//キャラクタ位置
	int			moveCnt_;	//行動カウンタ
	//向き（2D視点）
	float angle_;

	enum class Angle_LR
	{
		Left,Right
	};
	Angle_LR angle_LR_;
protected:
	//コンポーネント
	shared_ptr<BoxCollisionComponent> box_;
	class shared_ptr<Movement> movement_;
	class shared_ptr<Movement> gravity_;

public:
	Actor();
	virtual  ~Actor() {}

public:
	bool OutOfScreen() const;


	//あたり判定
	bool CheckHit(const ML::Box2D& hit) const;

	//めり込まない移動
	void CheckMove(ML::Vec2& move);

	BoxCollisionComponent* GetBox() const;
	Movement* GetMovement() const;
	Movement* GetGravity() const;
};