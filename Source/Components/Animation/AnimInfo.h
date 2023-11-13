#pragma once

#include "../../../GameEngine_Ver3_83.h"

class AnimInfo
{
	ML::Box2D		draw_;
	ML::Box2D		src_;
	ML::Color		color_;
	int maxSplit_;				//�ő啪����
	int splitIndex_;			//�\������Z��
	int switchTime_;			//�؂�ւ�鎞��
	vector<int> indexTable_;	//�C���f�b�N�X�̃e�[�u��
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