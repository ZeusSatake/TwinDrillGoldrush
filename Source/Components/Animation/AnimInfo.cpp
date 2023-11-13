#include "AnimInfo.h"
#include <assert.h>

AnimInfo::AnimInfo(const ML::Box2D& draw, const ML::Box2D& src, const ML::Color& color, const int maxSplit, const int switchTime)
	:
	draw_(draw),
	src_(src),
	color_(color),
	maxSplit_(maxSplit),
	splitIndex_(0),
	switchTime_(switchTime)
{}

AnimInfo::AnimInfo(const ML::Box2D& draw, const ML::Box2D& src, const ML::Color& color, const int maxSplit, const int indexTable[], size_t tableSize, const int switchTime)
	:
	AnimInfo(draw, src, color, maxSplit, switchTime)
{
	if (indexTable != nullptr) {

		for (int i = 0; i < tableSize; ++i) {
			if (indexTable[i] >= maxSplit || indexTable[i] < 0)
				assert(!"�A�j���[�V�����̃C���f�b�N�X�e�[�u���̗v�f���͈͊O�ł�");
			this->indexTable_.push_back(indexTable[i]);
		}
	}
}

void AnimInfo::GoNextSplitIndex()
{
	++this->splitIndex_;

	//�ő�l
	int maxIndex = indexTable_.empty() ?	//�C���f�b�N�X�̃e�[�u�������݂��邩�H
		maxSplit_ :					//�ő啪����
		(int)indexTable_.size();	//�e�[�u���̍ő吔

	//�C���f�b�N�X�𐳋K��
	if (splitIndex_ >= maxIndex)
		splitIndex_ %= maxIndex;
}

int AnimInfo::ProcessedNowIndex() const
{
	int rawIndex = splitIndex_;

	if (!indexTable_.empty())
		return indexTable_[rawIndex];

	return rawIndex;
}

ML::Box2D AnimInfo::GetSrc() const
{
	int index = ProcessedNowIndex();

	ML::Box2D result = src_;
	result.x += result.w * index;

	return result;
}

ML::Box2D AnimInfo::GetDraw() const
{
	return draw_;
}

int AnimInfo::GetSwitchTime() const
{
	return switchTime_;
}

void AnimInfo::ResetIndex()
{
	this->splitIndex_ = 0;
}