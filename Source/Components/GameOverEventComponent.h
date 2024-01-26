#pragma once
#include "../../Component.h"
#include <string>
#include <memory>
#include <functional>

namespace EventEngine { class Object; }

class GameOverEventComponent : public Component
{
	bool gameOver_;
	std::string eventPath_;
	std::shared_ptr<class SecondsTimerComponent> waitTimer_;
	std::weak_ptr<EventEngine::Object> event_;
	std::function<bool(void)> predicate_;

public:
	GameOverEventComponent(class Scene* owner, const std::string& eventPath);
	GameOverEventComponent(class Scene* owner, const std::string& eventPath, const float waitTime);
	virtual ~GameOverEventComponent() {};
	void Update() override;

	void SetWaitTime(const float time);
	void SetEventPath(const std::string& eventPath);
	void SetPred(const std::function<bool(void)>& predicate);

	void CheckGameOver();
	void ReadyGameOverEvent();
	void StartGameOverEvent();
	bool IsEndOfGameOverEvent() const;
};