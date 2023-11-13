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
		assert(!"�g���Ȃ�ID�ł��B����ID���g���ēo�^���ĉ������B");
	}

	//���łɑ��݂���
	if (this->drawInfos_.count(animId)) {
		assert(!"����ID�̃A�j���[�V�������o�^�ς݂ł��BID�̊m�F�����Ă��������B");
	}

	this->drawInfos_.emplace(animId, drawInfo);
}

AnimInfo& AnimManager::GetAnim(const int animId)
{
	//���݂��Ȃ��ꍇ�͑O�ɎQ�Ƃ��Ă������̂�Ԃ�
	if (drawInfos_.count(animId) == 0) {
		//�A�j���[�V����������o�^����Ă��Ȃ��ꍇ�̓G���[
		if (preId_ == nullId_) {
			assert(!"�A�j���[�V�������o�^����Ă��܂���B�o�^���Ă���Ăяo���Ă��������B");
		}

		return drawInfos_.at(preId_);
	}

	//�O��ƈႤ�A�j���[�V�������Q�Ƃ���ꍇ�̓J�E���g�����Z�b�g
	if (preId_ != animId) {
		animCount_ = 0;
	}

	preId_ = animId;
	return drawInfos_.at(animId);
}

AnimInfo AnimManager::Play(const int rawAnimId)
{

	//���݂��Ȃ��A�j���[�V�������w�肵���ꍇ�͑O�̂�Ԃ�
	//if (drawInfos_.count(rawAnimId) == 0) {
	//	assert(!"�Q�Ƃ��悤�Ƃ��Ă���A�j���[�V�����͑��݂��Ȃ����̂ł�");
	//}

	const int animId = drawInfos_.count(rawAnimId) == 0 ? //�w�肵���A�j���[�V���������݂��邩�H
		defalutAnimId_ :		//�f�t�H���g�̂��̂��g��
		rawAnimId;			//����̂��̂��g��

	//�Q�Ƃ���A�j���[�V�������ς���Ă�����J�E���g�����Z�b�g
	if (preId_ == nullId_ || preId_ != animId) {

		//���ɎQ�Ƃ������ɍŏ�����Đ����邽�߁A
		//�O��Q�Ƃ����A�j���̃C���f�b�N�X�����Z�b�g���Ă���
		if (preId_ != nullId_) {
			GetAnim(preId_).ResetIndex();
		}

		preId_ = animId;
		animCount_ = 0;
		return GetAnim(animId);
	}


	AnimInfo& animInfo = GetAnim(animId);

	++animCount_;
	//�J�E���g���ؑ֎��Ԉȏ�ɂȂ����玟�̃A�j����
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