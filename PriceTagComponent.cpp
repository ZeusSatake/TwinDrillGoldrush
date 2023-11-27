#include "PriceTagComponent.h"
#include <assert.h>

int PriceTagComponent::lastID = 0;

PriceTagComponent::PriceTagComponent(GameObject* owner)
	:
	Component((Actor*)owner),
	name_(""),
	price_(0),
	id_(lastID)
{
	++lastID;
}

void PriceTagComponent::Update()
{

}

void PriceTagComponent::SetPrice(const int price)
{
	if (price < 0)
		assert(!"設定しようとしている値段が0未満です。\nPriceTagComponentのSetPriceを呼び出している箇所を確認してください。");

	price_ = price;
}
void PriceTagComponent::SetName(const std::string& name)
{
	name_ = name;
}
void PriceTagComponent::Set(const std::string& name, const int price)
{
	SetName(name);
	SetPrice(price);
}

//=======精算=======

int PriceTagComponent::CalcTotalPrice(const PriceTagComponent& priceTag, const int num)
{
	return priceTag.price_ * num;
}
int PriceTagComponent::CalcTotalPrice(const std::map<PriceTagComponent, int> priceTags)
{
	int total = 0;
	for (const auto& priceTag : priceTags)
	{
		total += PriceTagComponent::CalcTotalPrice(priceTag.first, priceTag.second);
	}

	return total;
}
int PriceTagComponent::CalcTotalPrice(const int num) const
{
	return PriceTagComponent::CalcTotalPrice(*this, num);
}
//==================