#pragma once
#include "BPlayer.h"

class Player :public BPlayer 
{

public:
	Player();
	virtual ~Player() {}; 
	
	void PlayerMove(ML::Vec2 vec);//�x�N�g��������Ƃ��̕��v���C���[�𓮂���

	void Move() override;
};