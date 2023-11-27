#pragma once
#include "Component.h"
#include <map>
#include <string>
//�l�D�ł�����

class PriceTagComponent : public Component
{
	int price_;
	std::string name_;
	const int id_;
	static int lastID;
public:
	PriceTagComponent(class GameObject* owner);
	virtual ~PriceTagComponent() {};
	virtual void Update() override;

	void SetPrice(const int price);
	void SetName(const std::string& name);
	void Set(const std::string& name, const int price);

	//=======���Z=======

	//�l�D�ƌ�
	static int CalcTotalPrice(const PriceTagComponent& priceTag, const int num);
	//�����̒l�D�ƌ�
	static int CalcTotalPrice(const std::map<PriceTagComponent, int> priceTags);
	//��
	int CalcTotalPrice(const int num) const;

	//=================
};