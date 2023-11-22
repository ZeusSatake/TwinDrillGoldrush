#pragma once
#include "../../Component.h"
#include "../../MyPG.h"
#include "../../XI2018_Ver1_0.h"

class ControllerInputComponent :public Component //コントローラーの入力情報関係
{
public:
	XI::GamePad::SP gamePad_;
	ControllerInputComponent(class Actor* actor);

	void SetGamePadId(XI::GamePad::SP controller_);//どのコントローラーを持つか決めるセッター

	float GetAngleLStick();                        //左スティックの角度を取り出す
	float GetAngleRStick();                        //右スティックの角度を取り出す

	ML::Vec2 GetLStickVec();
	ML::Vec2 GetRStickVec();

};