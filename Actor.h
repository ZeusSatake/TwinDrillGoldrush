#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//ゲーム内オブジェクト基底クラス
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include "Source/Components/BoxCollisionComponent.h"
#include "Source/Components/HP.h"


class Actor : public GameObject
{
	ML::Vec2 moveVec_;
	float gravity_;
	float maxfallSpeed_;
	float jumpPower_;
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
	Angle_LR angle_LR;
protected:
	//コンポーネント
	shared_ptr<BoxCollisionComponent> box_;

public:
	Actor();
	virtual  ~Actor() {}
	ML::Vec2 GetMoveVec() const;
	void SetMoveVec(const ML::Vec2 vec);

	float GetGravity() const;
	void SetGravity(const float gravity);

	float GetMaxFallSpeed() const;
	void SetMaxFallSpeed(const float maxFallSpeed);

	float GetJumpPower() const;
	void SetJumpPower(const float jumpPower);

public:
	bool OutOfScreen() const;


	//あたり判定
	bool CheckHit(const ML::Box2D& hit) const;


};