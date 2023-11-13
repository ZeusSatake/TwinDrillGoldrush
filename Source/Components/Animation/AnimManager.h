#pragma once

#include "../../../GameEngine_Ver3_83.h"
#include "AnimInfo.h"

//�A�j���[�V�������Ǘ�����N���X
/*
	��)
		enum State {
			Idle,
			Attack,
			Walk
		}
		State nowState;

�E�C���X�^���X�쐬���Ƀf�t�H���g�̃A�j���[�V������ݒ� 
�@�o�^����ĂȂ����̂��Ă΂ꂽ�Ƃ��Ƀf�t�H���g�̂��Ă΂��
		AnimManager anim((int)State::Walk);

�E�������������ŃA�j���[�V������o�^�@AddAnim��2��
		nowState = Idle;

		0~�����������[�v
		anim.AddAnim((int)State::Atttack, DrawInfo(draw, src, color, ������, �؂�ւ�莞��));

		//�w�肵���e�[�u���ʂ�Ƀ��[�v(�e�[�u���͈͈̔͂ꉞ�`�F�b�N���Ă�)
		int attackAnimTable[] = { 0, 1, 2, 1 };
		anim.AddAnim((int)State::Walk, DrawInfo(draw, src, color, ������, attackAnimTable, size(attackAnimTable), �؂�ւ�莞��));
		

�E�`�揈�����ŕ`������擾���ĕ`��
		DrawInfo di = anim.Play((int)nowState));	//State::Idle�̎��̓A�j���[�V�������o�^����Ă��Ȃ����߁A
													//�f�t�H���g�ɐݒ肵��State::Walk���Ă΂��
		ML::Box2D draw = di.GetDraw().OffsetCopy(pos);//�����W�͎w�肷��K�v����
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