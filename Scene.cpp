#include "Scene.h"
#include "Source/UIBase/ToggleButton.h"

#include "Source/Scene/TitleScene.h"
#include "Source/Scene/EndingScene.h"
#include "Source/Scene/GameScene.h"
#include "Source/Scene/ShopScene.h"
#include "Source/Scene/BaseScene.h"
#include "Source/Scene/MartialFightScene.h"
#include "Source/Scene/OpeningScene.h"
#include "Source/Actors/Task_Player.h"

void Scene::SetNextScene(const Kind nextScene)
{
	nextScene_ = nextScene;
}

void Scene::AddSceneChangeButton(const shared_ptr<ToggleButton>& sceneChangeButton)
{
	sceneChangeButtons_.push_back(sceneChangeButton);
}

void Scene::CreateNextScene()
{
	if (ge->playerPtr != nullptr)
		ge->playerPtr->SceneTransitionInitialize(nextScene_);
	switch (nextScene_)
	{
	
	case Kind::Title:
		TitleScene::Object::Create(true);
		break;

	case Kind::Base:
		BaseScene::Object::Create(true);
		break;

	case Kind::Shop:
		ShopScene::Object::Create(true);
		break;

	case Kind::Mining:
		GameScene::Object::Create(true);
		break;

	case Kind::MartialFight:
		MartialFightScene::Object::Create(true);
		break;

	case Kind::Ending:
		EndingScene::Object::Create(true);
		break;

	case Kind::Opening:
		OpeningScene::Object::Create(true);
		break;

	default:
		assert(!"Scene.cppのCreateNextSceneで設定されていないシーンなので、次のシーンを作成できません。");
	}
}

void Scene::UpDate()
{
	for (const auto& button : sceneChangeButtons_)
	{
		if (button->IsPressed())
			Kill();
	}
}

Scene::Scene()
{}

Scene::~Scene()
{}