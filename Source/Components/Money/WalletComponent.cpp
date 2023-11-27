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
		assert(!"�x���z���}�C�i�X�ɂȂ��Ă��܂�\nWalletComponent��Payment()���Ăяo���Ă���ӏ����m�F���Ă��������B");
	
	if (balance_ < price)
		return false;

	balance_ -= price;
	return true;
}

bool WalletComponent::Recieve(const int price)
{
	if (price < 0)
		assert(!"�󂯎��z���}�C�i�X�ɂȂ��Ă��܂�\nWalletComponent��Recieve()���Ăяo���Ă���ӏ����m�F���Ă��������B");

	balance_ += price;
	return true;
}

int WalletComponent::GetBalance() const
{
	return balance_;
}