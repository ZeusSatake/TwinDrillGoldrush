#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ブロックの地盤
//-------------------------------------------------------------------
#include	"Actor.h"

class BBlocks : public Actor
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BBlocks>		   SP;
	typedef weak_ptr<BBlocks>		   WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2    pos;		//ブロック座標
	ML::Box2D   hitBase;	//あたり判定範囲

private:

public:
	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BBlocks()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
	{
	}
	virtual  ~BBlocks() {}
	
	//=================================

protected:

public:

	//受ける
	virtual void Received(BBlocks* object);
	virtual void Received(const int pow);
	virtual bool Drilling(const string& targetGroupName);
	virtual bool Drilling(const string& targetGroupName, const ML::Box2D& attackArea);
	void LoadSe();


};