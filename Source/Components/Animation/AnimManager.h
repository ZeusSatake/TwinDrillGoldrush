#pragma once

#include "../../../GameEngine_Ver3_83.h"
#include "AnimInfo.h"

//アニメーションを管理するクラス
/*
	例)
		enum State {
			Idle,
			Attack,
			Walk
		}
		State nowState;

・インスタンス作成時にデフォルトのアニメーションを設定 
　登録されてないものが呼ばれたときにデフォルトのが呼ばれる
		AnimManager anim((int)State::Walk);

・初期化処理内でアニメーションを登録　AddAnimは2種
		nowState = Idle;

		0~分割数をループ
		anim.AddAnim((int)State::Atttack, DrawInfo(draw, src, color, 分割数, 切り替わり時間));

		//指定したテーブル通りにループ(テーブルの範囲は一応チェックしてる)
		int attackAnimTable[] = { 0, 1, 2, 1 };
		anim.AddAnim((int)State::Walk, DrawInfo(draw, src, color, 分割数, attackAnimTable, size(attackAnimTable), 切り替わり時間));
		

・描画処理内で描画情報を取得して描画
		DrawInfo di = anim.Play((int)nowState));	//State::Idleの時はアニメーションが登録されていないため、
													//デフォルトに設定したState::Walkが呼ばれる
		ML::Box2D draw = di.GetDraw().OffsetCopy(pos);//※座標は指定する必要あり
		img->Draw(draw, di.GetSrc());
*/

class AnimManager
{
	map<int, AnimInfo> drawInfos_;
	static const int nullId_;
	int animCount_;
	int preId_;
	int defalutAnimId_;
	AnimInfo& GetAnim(const int animId);
public:
	AnimManager(const int defalutAnimId);
	void AddAnim(const int animId, const AnimInfo& drawInfo);
	AnimInfo Play(const int animId);
	void AllClear();
	void SetDefaultAnimId(const int defalutAnimId);
};