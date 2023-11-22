#pragma once
#include "NPC.h"
#include "myLib.h"
class Enemy : public NPC
{
public:
	float range_;//ŽË’ö
public:
	Enemy();
	virtual ~Enemy() {};

	float GetRange() const;
	void SetRange(const float range);
};

