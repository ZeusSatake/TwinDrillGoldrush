#pragma once
#include "NPC.h"
#include "myLib.h"
class Enemy : public NPC
{
	float range_;//�˒�
public:
	Enemy();
	virtual ~Enemy() {};

	float GetRange() const;
	void SetRange(const float range);
};

