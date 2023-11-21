#pragma once
#include "BPlayer.h"

class Player :public BPlayer 
{

public:
	Player();
	virtual ~Player() {}; 
	
	void PlayerMove(ML::Vec2 vec);//ベクトルを入れるとその分プレイヤーを動かす

	void Move() override;
};