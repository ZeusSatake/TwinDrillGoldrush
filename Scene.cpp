#include "Scene.h"
#include "ToggleButton.h"

#include "Source/Scene/TitleScene.h"
#include "Source/Scene/EndingScene.h"
#include "Source/Scene/Task_Game.h"
#include "Source/Scene/ShopScene.h"

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
	switch (nextScene_)
	{
	
	case Kind::Title:
		TitleScene::Object::Create(true);
		break;

	case Kind::Base:
		Game::Object::Create(true);
		break;

	case Kind::Shop:
		ShopScene::Object::Create(true);
		break;

	case Kind::Mining:
		break;

	case Kind::MartialFight:
		break;

	case Kind::Ending:
		EndingScene::Object::Create(true);
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