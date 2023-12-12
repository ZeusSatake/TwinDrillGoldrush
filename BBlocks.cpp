#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ブロックの地盤
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"BBlocks.h"
#include	"sound.h"

void BBlocks::LoadSe()
{
	se::LoadFile("crush", "./data/sound/crush.wav");
	se::LoadFile("repelled", "./data/sound/repelled.wav");
}

//ブロックそれぞれの処理
void BBlocks::Received(BBlocks* object)
{
}
void BBlocks::Received(const int pow)
{
}


bool BBlocks::Drilling(const string& targetGroupName)
{
	bool successAttack = false;
	auto targets = ge->GetTasks<BBlocks>(targetGroupName);

	if (targets == nullptr)
		return successAttack;

	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);

	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//ダメージ処理
		if ((*it)->CheckHit(me)) {
			(*it)->Received(this);
			successAttack = true;
		}
	}

	return successAttack;
}
bool BBlocks::Drilling(const string& targetGroupName, const ML::Box2D& attackArea)
{
	bool successAttack = false;
	auto targets = ge->GetTasks<BBlocks>(targetGroupName);

	if (targets == nullptr)
		return successAttack;

	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//ダメージ処理
		if ((*it)->CheckHit(attackArea)) {
			(*it)->Received(this);
			successAttack = true;
		}
	}

	return successAttack;
}

void BBlocks::getMapName(const string MapName_)
{
	string str = MapName_;
	MapName = str;
}