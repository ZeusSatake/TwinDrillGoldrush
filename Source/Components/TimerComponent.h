#pragma once
#include "../../Component.h"
#include "../../GameObject.h"
//カウントを開始したい場所でTimerのStart()を呼び出す
//タスクのUpdate内などでTimerのUpdate()を呼び出す
class TimerComponent : public Component
{
	int nowCount_;		//時間カウンタ
	int countFrame_;	//カウントするフレーム
	bool isCounting_;	//カウント中
	//カウント中か判定
	void JudgeCount();
public:
	TimerComponent(class GameObject* owner);
	TimerComponent(class GameObject* ownerint, int nowCount, int countFrame);

	//現在のカウント取得
	int  GetCount();
	//カウント中
	bool IsCounting();
	//カウントするフレーム数を設定
	void SetCountFrame(int countFrame);
	//無敵時間開始
	void Start();
	//更新
	void Update();
};