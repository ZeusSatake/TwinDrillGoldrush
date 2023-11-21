#pragma once
#include "AIComponent.h"
#include "../../Debtor.h"
//債務者AIコンポーネント
class DebtorAIComponent : public AIComponent
{
public:
	DebtorAIComponent(class Debtor* owner);
	virtual ~DebtorAIComponent() {};



};

