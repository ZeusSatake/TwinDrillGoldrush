#pragma once
#include "AIComponent.h"
#include "../../Debtor.h"
//����AI�R���|�[�l���g
class DebtorAIComponent : public AIComponent
{
public:
	DebtorAIComponent(class Debtor* owner);
	virtual ~DebtorAIComponent() {};



};

