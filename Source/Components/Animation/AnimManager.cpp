#include "AnimManager.h"
#include <assert.h>

const int AnimManager::nullId_ = -10000;

AnimManager::AnimManager(const int defalutAnimId)
	:
	animCount_(0),
	preId_(nullId_),
	defalutAnimId_(defalutAnimId)
{
}

void AnimManager::AddAnim(const int animId, const AnimInfo& drawInfo)
{
	if (animId == nullId_) {
		assert(!"使えないIDです。他のIDを使って登録して下さい。");
	}

	//すでに存在する
	if (this->drawInfos_.count(animId)) {
		assert(!"同じIDのアニメーションが登録済みです。IDの確認をしてください。");
	}

	this->drawInfos_.emplace(animId, drawInfo);
}

AnimInfo& AnimManager::GetAnim(const int animId)
{
	//存在しない場合は前に参照していたものを返す
	if (drawInfos_.count(animId) == 0) {
		//アニメーションが一つも登録されていない場合はエラー
		if (preId_ == nullId_) {
			assert(!"アニメーションが登録されていません。登録してから呼び出してください。");
		}

		return drawInfos_.at(preId_);
	}

	//前回と違うアニメーションを参照する場合はカウントをリセット
	if (preId_ != animId) {
		animCount_ = 0;
	}

	preId_ = animId;
	return drawInfos_.at(animId);
}

AnimInfo AnimManager::Play(const int rawAnimId)
{

	//存在しないアニメーションを指定した場合は前のを返す
	//if (drawInfos_.count(rawAnimId) == 0) {
	//	assert(!"参照しようとしているアニメーションは存在しないものです");
	//}

	const int animId = drawInfos_.count(rawAnimId) == 0 ? //指定したアニメーションが存在するか？
		defalutAnimId_ :		//デフォルトのものを使う
		rawAnimId;			//今回のものを使う

	//参照するアニメーションが変わっていたらカウントをリセット
	if (preId_ == nullId_ || preId_ != animId) {

		//次に参照した時に最初から再生するため、
		//前回参照したアニメのインデックスをリセットしておく
		if (preId_ != nullId_) {
			GetAnim(preId_).ResetIndex();
		}

		preId_ = animId;
		animCount_ = 0;
		return GetAnim(animId);
	}


	AnimInfo& animInfo = GetAnim(animId);

	++animCount_;
	//カウントが切替時間以上になったら次のアニメへ
	if (animCount_ >= animInfo.GetSwitchTime()) {
		GetAnim(rawAnimId).GoNextSplitIndex();
		animCount_ = 0;
	}
	return animInfo;
}

void AnimManager::AllClear()
{
	drawInfos_.clear();
}

void AnimManager::SetDefaultAnimId(const int defalutAnimId)
{
	this->defalutAnimId_ = defalutAnimId;
}