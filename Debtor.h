#pragma once
#include "Enemy.h"
#include "Source/Components/DebtorAIComponent.h"
//-----------------------------------------------------
//���Ҋ��N���X
//-----------------------------------------------------
class Debtor : public Enemy
{
public:
	Debtor();
	virtual ~Debtor() {};
protected:
	class shared_ptr<DebtorAIComponent> AI_;
	
};

