#include "Debtor.h"

Debtor::Debtor()
	:Enemy()
{
	AddComponent(AI_ = shared_ptr<DebtorAIComponent>(new DebtorAIComponent(this)));
}
