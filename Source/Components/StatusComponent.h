#pragma once
#include "../../Component.h"
#include "../Components/HP.h"
template<typename T>
class StatusComponent : public Component
{
	template<typename> friend class StatusComponent;
	T now_;
	T max_;
public:
	StatusComponent(Actor* owner)
		: Component(owner)
	{
	}
	StatusComponent(Actor* owner, const T max)
		: Component(owner)
		, max_(max)
	{
	}
	virtual ~StatusComponent() {};

	template<typename T>
	T Get()
	{
		return now_;
	}

	T GetMax(const T val)
	{
		return max_;
	}

	template<typename T>
	void SetMax(const T val)
	{
		max_ = val;
	}
	void Add(const T addVal)
	{
		now_ += addVal;
	}

	void Initialize(const T now, const T max)
	{
		now_ = now;
		max_ = max;
	}
};

