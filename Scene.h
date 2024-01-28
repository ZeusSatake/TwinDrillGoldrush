#pragma once
#include "Actor.h"

class ToggleButton;

class Scene : public GameObject
{
public:
	shared_ptr<Scene> SP;
	weak_ptr<Scene> WP;

	enum Kind
	{
		Title,
		Base,
		Shop,
		Mining,
		MartialFight,
		Ending,
		Opening
	};
private:
	Kind nextScene_;
	vector<shared_ptr<ToggleButton>> sceneChangeButtons_;
public:
	void SetNextScene(const Kind nextScene);
	void AddSceneChangeButton(const shared_ptr<ToggleButton>& sceneChangeButton);
	void UpDate() override;
	void CreateNextScene();
	Scene();
	virtual ~Scene();
};

