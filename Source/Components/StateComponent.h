#pragma once
#include"../../Component.h"
//-----------------------------------------------------------------------
// ステートマシン
//-----------------------------------------------------------------------
class StateComponent : public Component
{
	enum class State
	{
		Non,        //無し
		Idle,		//待機
		Walk,		//歩き
		Attack,		//攻撃
		SpinAttack, //回転攻撃
		Damage,	    //ダメージ
		KnockBack,  //のけぞり
		Dead,		//死亡
		Jump,       //ジャンプ
		Fall,       //降下
		Dash,       //ダッシュ
		DrillDash,  //ドリルダッシュ
		Mining,     //採掘
		Appeal      //アピール
	};
	State nowState;
	State preState;
public:
	StateComponent(class Actor* actor);
	void UpdateNowState(State state); //ステータスの更新
	void InitializeState(State state);//ステータスの初期化
	State GetNowState();              //nowStateのゲッター

};