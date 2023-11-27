#include "WalletComponent.h"
#include <assert.h>

WalletComponent::WalletComponent(class GameObject* owner)
	:
	Component((Actor*)owner),
	balance_(0)
{

}

void WalletComponent::Update()
{

}

bool WalletComponent::Payment(const int price)
{
	if (price < 0)
		assert(!"支払額がマイナスになっています\nWalletComponentのPayment()を呼び出している箇所を確認してください。");
	
	if (balance_ < price)
		return false;

	balance_ -= price;
	return true;
}

bool WalletComponent::Recieve(const int price)
{
	if (price < 0)
		assert(!"受け取り額がマイナスになっています\nWalletComponentのRecieve()を呼び出している箇所を確認してください。");

	balance_ += price;
	return true;
}

int WalletComponent::GetBalance() const
{
	return balance_;
}