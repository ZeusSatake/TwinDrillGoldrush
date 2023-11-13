#pragma once

#include "../../../GameEngine_Ver3_83.h"

class AnimInfo
{
	ML::Box2D		draw_;
	ML::Box2D		src_;
	ML::Color		color_;
	int maxSplit_;				//最大分割数
	int splitIndex_;			//表示するセル
	int switchTime_;			//切り替わる時間
	vector<int> indexTable_;	//インデックスのテーブル
public:
	AnimInfo(const ML::Box2D& draw, const ML::Box2D& src, const ML::Color& color, const int maxSplit, const int indexTable[], size_t tableSize, int switchTime);
	AnimInfo(const ML::Box2D& draw, const ML::Box2D& src, const ML::Color& color, const int maxSplit, int switchTime);

	void GoNextSplitIndex();

	int ProcessedNowIndex() const;

	ML::Box2D GetSrc() const;
	ML::Box2D GetDraw() const;
	int GetSwitchTime() const;
	void ResetIndex();
};