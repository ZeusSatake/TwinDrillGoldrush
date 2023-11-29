#pragma once
#include "ToggleButton.h"

class Timer;

class PushButton : public ToggleButton
{
	float resetTime_sec_;
	shared_ptr<Timer> resetTimer_;
public:
	PushButton();
	virtual ~PushButton(){};

	void OnEvent() override;
	void OffEvent() override;

	void UpDate() override;

	void SetResetTime(const float resetTime_sec, const )
	const shared_ptr<Timer> GetTimer() const;
};