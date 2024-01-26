#include "GameOverEventComponent.h"

#include "../../Scene.h"

#include "SecondsTimerComponent.h"

#include "../Actors/Task_Player.h"
#include "../Event/Task_EventEngine.h"

GameOverEventComponent::GameOverEventComponent(Scene* owner, const std::string& eventPath)
	:
	GameOverEventComponent(owner, eventPath, 0.0f)
{
}
GameOverEventComponent::GameOverEventComponent(Scene* owner, const std::string& eventPath, const float waitTime)
	:
	Component((Actor*)owner),
	gameOver_(false),
	eventPath_(eventPath),
	waitTimer_(make_shared<SecondsTimerComponent>(owner)),
	predicate_(nullptr)
{
	waitTimer_->SetCountSeconds(waitTime);
}

void GameOverEventComponent::Update()
{
	waitTimer_->Update();

	if (predicate_ == nullptr)
		assert(!"ゲームオーバー条件が設定されていません。");
	gameOver_ = predicate_();

	//CheckGameOver();
	if (gameOver_)
		ReadyGameOverEvent();

	if (waitTimer_->IsCountEndFrame())
		StartGameOverEvent();

	if (IsEndOfGameOverEvent())
		owner_->Kill();
}

void GameOverEventComponent::SetWaitTime(const float time)
{
	waitTimer_->SetCountSeconds(time);
}
void GameOverEventComponent::SetEventPath(const std::string& eventPath)
{
	eventPath_ = eventPath;
}
void GameOverEventComponent::SetPred(const std::function<bool(void)>& predicate)
{
	predicate_ = predicate;
}

//void GameOverEventComponent::CheckGameOver()
//{
//	if (ge->playerPtr->GetStatus()->HP.IsAlive())
//		return;
//
//	gameOver_ = true;
//}

void GameOverEventComponent::ReadyGameOverEvent()
{
	if (waitTimer_->IsActive())
		return;
	if (waitTimer_->WasCountEnd())
		return;
	waitTimer_->Start();
}

void GameOverEventComponent::StartGameOverEvent()
{
	event_ = EventEngine::Object::Create_Mutex();
	event_.lock()->Set(eventPath_);
}

bool GameOverEventComponent::IsEndOfGameOverEvent() const
{
	if (!gameOver_)
		return false;
	if (!waitTimer_->WasCountEnd())
		return false;
	if (event_.lock())
		return false;

	return true;
}