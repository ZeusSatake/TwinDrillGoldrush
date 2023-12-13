#pragma once
#include "ToggleButton.h"

class SecondsTimerComponent;

class PushButton : public ToggleButton
{
	float resetTime_sec_;
	shared_ptr<SecondsTimerComponent> resetTimer_;

	int resetCount = 0;
	bool enableResetTimer_;
public:
	PushButton();
	virtual ~PushButton() {};

	void OnEvent() override;
	void OffEvent() override;

	void UpDate() override;

	void SetResetTime(const float resetTime_sec);
	const shared_ptr<SecondsTimerComponent> GetTimer() const;
	void SetEnable_ResetTimer(const bool enable);
	void SetEnable(const bool enable);
};