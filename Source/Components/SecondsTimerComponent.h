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
	bool wasCountEnd_;		//カウント終了後
	//カウント中か判定
	void SetActive();
public:
	SecondsTimerComponent(class GameObject* owner);
	SecondsTimerComponent(class GameObject* ownerint, const float countSeconds);

	void SetCountSeconds(const float countSeconds);

	//現在のカウント取得
	float  GetCount() const;
	//カウント中
	bool IsActive() const;
	//カウント終了フレームのみtrue
	bool IsCountEndFrame() const;
	//カウント終了後のみtrue
	bool WasCountEnd() const;

	//タイマー起動
	void Start();

	void Update() override;
};