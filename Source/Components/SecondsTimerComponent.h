#pragma once
#include "../../Component.h"
#include "../../GameObject.h"

//カウントを開始したい場所でTimerのStart()を呼び出す
//IsActive()がtrueの時はカウント中
class SecondsTimerComponent : public Component
{
	float nowCount_;		//時間カウンタ
	float countSeconds_;	//カウントする秒数
	bool isActive_;			//カウント中
	bool isCountEndFrame_;	//カウント終了フレーム
	//カウント中か判定
	void SetActive();
public:
	SecondsTimerComponent(class GameObject* owner);
	SecondsTimerComponent(class GameObject* ownerint, const float nowCount, const float countSeconds);

	void SetCountSeconds(const float countSeconds);

	//現在のカウント取得
	int  GetCount() const;
	//カウント中
	bool IsActive() const;
	bool IsCountEndFrame() const;
	//無敵時間開始
	void Start();
	//更新
	void Update() override;
};