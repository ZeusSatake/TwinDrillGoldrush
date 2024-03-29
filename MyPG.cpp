﻿#include "MyPG.h"

#include "randomLib.h"
#include "sound.h"
#define MYDEBUG


#include "Source/Scene/TitleScene.h"
#include "Source/Actors/Task_Player.h"

namespace MyPG
{
	shared_ptr<player::Object> MyGameEngine::playerPtr;

	void MyGameEngine::InitCounter() {
		counterMap.clear();
	}
	void MyGameEngine::ActCounter() {
		for (auto& c : counterMap) {
			if (c.second.flag == COUNTER_FLAGS::LIMIT) {
				c.second.flag = COUNTER_FLAGS::END;
			}
			if (c.second.flag == COUNTER_FLAGS::ACTIVE) {
				c.second.count--;
				if (c.second.count <= 0) {
					c.second.flag = COUNTER_FLAGS::LIMIT;
				}
			}
		}
	}
	//すべてのカウンタを強制的に止める。
	void MyGameEngine::AllEndCounter() {
		for (auto& c : counterMap) {
			if (c.second.flag == COUNTER_FLAGS::ACTIVE) {
				c.second.flag = COUNTER_FLAGS::END;
			}
		}
	}
	//指定の名前のカウンタの状態を取得する。
	MyGameEngine::COUNTER_FLAGS MyGameEngine::getCounterFlag(string str)
	{
		return counterMap[str].flag;
	}
	//指定の名前をカウンタを作成し、指定フレームのカウントを始める。
	void MyGameEngine::StartCounter(string str, int count) {
		counterMap[str].count = count;
		counterMap[str].flag = COUNTER_FLAGS::ACTIVE;
	}


	void MyGameEngine::CreateEffect(int no, ML::Vec2 pos) {
		this->CreateEffect(no, pos, 0);
	}

	void MyGameEngine::CreateEffect(int no, ML::Vec2 pos,int id) {
		switch (no) {
		case 0: //上から落ちてくる10個の玉
			for (int i = 0; i < 10; i++) {
				if (auto e = Effect00::Object::Create(true)) {
					//利用するファイル
					e->filename = "data/image/effect/bomb.png";
					//元リソースの１コマのサイズ
					e->resSizeX = e->resSizeY = 800;
					//描画サイズ
					e->drawSizeX = e->drawSizeY = 50;
					//出現位置
					//e->pos.x = (float)(rand() % ge->screen2DWidth);
					//e->pos.y = (float)(ge->screen2DHeight+ 30);
					e->pos.x = GetRandom((float)0.f, (float)ge->screen2DWidth);
					e->pos.y = (float)(0);

					e->addAlpha = -0.005f;
					e->countMax = GetRandom(50, 100);
					e->graVec = ML::Vec2(0.f, GetRandom(0.1f, 3.f));

					//e->AddRender = true;
				}
			}
			break;
		case 1://小さくなる玉　爆発 指定座標
			if (auto e = Effect00::Object::Create(true)) {
				//利用するファイル
				e->filename = "data/image/effect/bomb.png";
				//元リソースの１コマのサイズ
				e->resSizeX = e->resSizeY = 800;
				//描画サイズ
				e->drawSizeX = e->drawSizeY = 30;
				//出現位置
				e->pos.x = pos.x;
				e->pos.y = pos.y;
				//拡大率　デフォルトが1なので本来は設定不要
				e->scale = 3.0f;
				//拡大率の加算
				e->addScale = -0.1f;
			}
			break;
		case 2://小さくなる玉　爆発 ランダム座標
			if (auto e = Effect00::Object::Create(true)) {
				//利用するファイル
				e->filename = "data/image/effect/bomb.png";
				//元リソースの１コマのサイズ
				e->resSizeX = e->resSizeY = 800;
				//描画サイズ
				e->drawSizeX = e->drawSizeY = 30;
				//出現位置
				e->pos.x = GetRandom((float)0.f, (float)ge->screen2DWidth);
				e->pos.y = GetRandom((float)0.f, (float)ge->screen2DHeight);
				//拡大率　デフォルトが1なので本来は設定不要
				e->scale = 3.0f;

				e->addAlpha = -0.02f;

			}
			break;

		case 3: //薄くなる霧 ランダム座標
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/myst.png";
				e->resSizeX = e->resSizeY = 80;
				e->drawSizeX = e->drawSizeY = 80;
				e->pos.x = GetRandom((float)0.f, (float)ge->screen2DWidth);
				e->pos.y = GetRandom((float)0.f, (float)ge->screen2DHeight);
				e->addAlpha = -0.06f;
			}
			break;
		case 4://薄くなる霧　左に移動する　ランダム座標
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/myst.png";
				e->resSizeX = e->resSizeY = 80;
				e->drawSizeX = e->drawSizeY = 80;
				e->pos.x = GetRandom((float)0.f, (float)ge->screen2DWidth);
				e->pos.y = GetRandom((float)0.f, (float)ge->screen2DHeight);
				e->addAlpha = -0.06f;

				e->moveVec.x = -2.0f;
				e->moveVec.y = 0.f;
			}
			break;
		case 5://GetReady 画面中央　1秒で消滅
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/getready.png";
				e->resSizeX = e->drawSizeX = 545;
				e->resSizeY = e->drawSizeY = 105;
				e->pos.x = (float)ge->screen2DWidth / 2;
				e->pos.y = (float)ge->screen2DHeight / 2;

				//1秒で消滅
				e->countMax = 60;
			}
			break;
		case 6://Start 画面中央　1秒で消滅
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/start.png";
				e->resSizeX = e->drawSizeX = 247;
				e->resSizeY = e->drawSizeY = 101;
				e->pos.x = (float)ge->screen2DWidth / 2;
				e->pos.y = (float)ge->screen2DHeight / 2;

				//1秒で消滅
				e->countMax = 60;
			}
			break;

		case 7: //薄くなる霧 座標指定　左へ
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/myst.png";
				e->resSizeX = e->resSizeY = 80;
				e->drawSizeX = e->drawSizeY = 40;
				e->pos.x = pos.x;
				e->pos.y = pos.y;
				e->addAlpha = -0.06f;
				e->moveVec.x = -2.0f;
				e->moveVec.y = 0.f;

				//拡大率　デフォルトが1なので本来は設定不要
				e->scale = 1.0f;
				e->addScale = 0.1f;


			}
			break;

		case 8://爆発　アニメーション(加算合成なし)
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/effect3_bomb1.png";
				e->resSizeX = 256;
				e->resSizeY = 256;
				e->drawSizeX = 512;
				e->drawSizeY = 512;
				e->pos.x = pos.x;
				e->pos.y = pos.y;

				e->animResNum = 10;
				e->animMax = 40;
				e->addAnim = 0.6f;

				//1秒で消滅
				e->countMax = 120;
			}
			break;

		case 9://爆発　アニメーション(加算合成あり)
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/effect3_bomb1.png";
				e->resSizeX = 256;
				e->resSizeY = 256;
				e->drawSizeX = 512;
				e->drawSizeY = 512;
				e->pos.x = pos.x;
				e->pos.y = pos.y;

				e->animResNum = 10;
				e->animMax = 40;
				e->addAnim = 0.6f;

				e->AddRender = true;

				//1秒で消滅
				e->countMax = 120;
			}
			break;

		case 10://小爆発　アニメーション(加算合成あり)
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/effect3_bomb1.png";
				e->resSizeX = 256;
				e->resSizeY = 256;
				e->drawSizeX = 32;
				e->drawSizeY = 32;
				e->pos.x = pos.x;
				e->pos.y = pos.y;

				e->animResNum = 10;
				e->animMax = 40;
				e->addAnim = 0.6f;

				e->AddRender = true;

				//1秒で消滅
				e->countMax = 120;
			}
			break;

		case 11://粉砕エフェクト
			for (int i = 0; i < 3; i++) {
				if (auto e = Effect00::Object::Create(true)) {
					string filePath = e->filename = "data/image/effect/Effect_Blocks/crushEffect_" + to_string(id) + ".png";
					ifstream ifs(filePath);
					if (!ifs) { e->filename = "data/image/effect/Effect_Blocks/crushEffect_" + to_string(0) + ".png";; }

					e->resSizeX = e->resSizeY = 32;
					e->drawSizeX = e->drawSizeY = 32;
					//元リソースの１コマのサイズ
					//e->resSizeX = e->resSizeY = 800;
					//描画サイズ
					//e->drawSizeX = e->drawSizeY = 50;

					e->pos.x = pos.x;
					e->pos.y = pos.y;
					e->scale = 0.3f;
					e->angle = GetRandom(30, 330);
					e->addAngle = GetRandom(10, 30);

					e->moveVec.x = GetRandom(-1.0f, 1.0f);
					e->moveVec.y = GetRandom(-3.0f, 3.0f);

					e->addAlpha = -0.005f;
					e->countMax = GetRandom(50, 100);
					e->graVec = ML::Vec2(0.f, 0.1f);
				}
			}
			break;

		case 99://フェードアウトしてからフェードイン 90フレーム
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/black.png";
				e->resSizeX = e->resSizeY = 256;
				e->drawSizeX = (int)ge->screen2DWidth;
				e->drawSizeY = (int)ge->screen2DHeight;
				e->pos.x = (float)ge->screen2DWidth / 2;
				e->pos.y = (float)ge->screen2DHeight / 2;
				e->fade = true;
				e->addAngle = 2.f;
				e->angle = 0.f;
				e->alpha = 0.f;
			}
			break;
		case 98://フェードアウトしてからフェードイン　90フレーム
			if (auto e = Effect00::Object::Create(true)) {
				e->filename = "data/image/effect/white.png";
				e->resSizeX = e->resSizeY = 256;
				e->drawSizeX = (int)ge->screen2DWidth;
				e->drawSizeY = (int)ge->screen2DHeight;
				e->pos.x = (float)ge->screen2DWidth / 2;
				e->pos.y = (float)ge->screen2DHeight / 2;
				e->fade = true;
				e->addAngle = 2.f;
				e->angle = 0.f;
				e->alpha = 0.f;
			}
			break;





		}

	}

	//---------------------------------------------------------------------------------------
	//ウインドウへメッセージの表示
	//	引数
	//		x,y			表示位置
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToDisplay(int x, int y, const char* str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		ML::Box2D draw(x, y, 480, 30);
		//        DG::Font::SP font = DG::Font::Create("HGS行書体", 12, 24, 100);
		this->debugFont->Draw(draw, buf, ML::Color(1, 1, 1, 1));
#else
		//空実装
#endif
	}

	//---------------------------------------------------------------------------------------
	//タイトルへメッセージの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToTitle(const char* str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		SetWindowText(GetActiveWindow(), (LPCSTR)buf);
#else
		//空実装
#endif
	}

	//---------------------------------------------------------------------------------------
	//メッセージボックスへの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToBox(const char* str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		MessageBox(NULL, (LPCSTR)buf, "Message", MB_OK);
#else
		//空実装
#endif
	}
	//---------------------------------------------------------------------------------------
	//コンソールへの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToConsole(const char* str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		OutputDebugString((LPCSTR)buf);
#else
		//空実装
#endif
	}
	//デバッグ矩形
//ここから
//HowToUse
/*
debugrect.pngをイメージフォルダにコピー
この関数群を自分のMyPG.cppにコピー
同じくプロトタイプ宣言周辺もMyPG.ｈへコピー

利用したいタスク（オススメはGameタスク）
bool  Object::Initialize()
{
	//プライオリティを上げないと描画されない。デバッグは最も手前でよい。
	this->render2D_Priority[1] = 0.0f;
	ge->debugRectLoad();
}
bool  Object::Finalize()
{
	ge->debugRectReset();
}
void  Object::Render2D_AF()
{
	ge->debugRectDraw();
}
で下準備おわり

あとは必要に応じて
//当たり判定
ge->debugRect(hit);
で指定したBox2Dに矩形が表示される。

色はデフォルトで赤塗りつぶし
第2引数で指定可能
enum DEBUGRECTMODE {
RED,BLUE,GREEN,ORANGE,
REDFRAME, BLUEFRAME, GREENFRAME, ORANGEFRAME,
};
ge->debugRect(hit,DEBUGRECTMODE::GREENFRAME);
で指定したBox2Dに矩形が表示される。

スクロールや座標などのオフセットはオフセットした後でよいが、
ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
//スクロール対応の場合は、こんな記述になる。
ge->debugRect(me, DEBUGRECTMODE::RED ,- ge->camera2D.x, -ge->camera2D.y);




*/
	void MyGameEngine::debugRectLoad()
	{
		ge->rectImage = DG::Image::Create("./data/effect/debugrect.png");
	}
	void MyGameEngine::debugRectReset()
	{
		ge->rectImage.reset();
	}

	void MyGameEngine::debugRect(ML::Box2D r_, int flag, int offsetx, int offsety) {
		ML::Box2D temp = r_.OffsetCopy(offsetx, offsety);
		DebugRectInfo d = { temp,flag };
		ge->debugRectArray.push_back(d);
	}

	void MyGameEngine::debugRectDraw() {
		for (auto& r : ge->debugRectArray) {
			ML::Box2D src((r.flag % 4) * 32, (r.flag / 4) * 32, 32, 32);
			ge->rectImage->Draw(r.r, src, ML::Color(1.0, 1, 1, 1));
		}
		ge->debugRectArray.clear();
	}
	//ここまで


	//---------------------------------------------------------------------------------------
	//ファイルOUT関数
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToFileOut(const char* str, ...) {
#ifdef MYDEBUG

		FILE* fp;
		char buff[128];
		vsprintf(buff, (char*)str, (char*)(&str + 1));
		strcat(buff, "\n");
		if ((fp = fopen("debug.txt", "at")) != NULL) {
			fprintf(fp, "%s", buff);
			fclose(fp);
		}
#else
		//空実装
#endif
	}
	MyGameEngine::MyGameEngine()
		:GameEngine(
			//	環境情報
			1280,							//	画面サイズX
			720,							//	画面サイズY
			1,								//	表示倍率
			1,								//	マルチサンプル(1or2or4)
			false,							//	フルスクリーンモード
			480,							//	2D描画用解像度X(現在非対応）
			270,							//	2D描画用解像度Y(現在非対応）
			"2023年度 卒業制作 オジョウサマイニング ～崩せ一点、目指せテッペン～ 嗚呼...ゼウス佐竹とわんぱくデストロイヤーズfeat.カルロス根岸",	//	ウインドウタイトル
			"GPG",							//	ウィンドウクラス名
			1,//UpDate呼び出し頻度　↓のRender呼び出し頻度との比率で決まる１以上を指定すること
			1 //Render呼び出し頻度　↑のUpDate呼び出し頻度との比率で決まる１以上を指定すること
			//例えば、120Hz固定のPCで、60Hzと同等の状態で動かす場合、1U/2Rで設定すれば、UpDateの呼び出しが1/2になる
		) {
		//seting.txtによるリフレッシュレート調整の為追加
		ifstream  fin("./data/Shader/seting.txt");
		if (fin) {
			fin >> this->updateCall >> this->renderCall;
			fin.close();
		}
	}

	//ゲームエンジンに追加したものの初期化と開放
	bool MyGameEngine::Initialize(HWND wnd_)
	{
		//　画像描画ライブラリの初期化
		{
			this->dgi = DG::DGObject::Create(wnd_, this->screenWidth, this->screenHeight, this->multiSample,
				this->screenMode, this->screen2DWidth, this->screen2DHeight);
			if (!this->dgi) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//	入力機器ライブラリの初期化
		{
			this->dii = XI::Obj::Create(wnd_);
			if (!this->dii) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//	サウンドライブラリの初期化
		{
			this->dmi = DM::Obj::Create(wnd_);
			if (!this->dmi) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//小柳サウンドライブラリ初期化
		{
			bgm::Init();
			se::Init(wnd_);

		}
		//ランダムライブラリ初期化
		{
			InitRandom();
		}
		//FPS計測
		{
			c = new FPSCounter(REFRESHRATE);
		}

		//デバッグ用フォントの準備
		this->debugFont = DG::Font::Create("ＭＳ ゴシック", 6, 16);

		//デフォルトカメラ
		this->camera[0] = MyPG::Camera::Create(
			D3DXVECTOR3(1.0f, 0.0f, 0.0f),	//	ターゲット位置
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//	カメラ位置
			D3DXVECTOR3(0.0f, 1.0f, 0.0f),		//	カメラの上方向ベクトル
			ML::ToRadian(60.0f), 10.0f, 300.0f,	//	視野角・視野距離
			(float)this->screenWidth / (float)this->screenHeight);		//	画面比率
		this->dgi->EffectState().param.bgColor = ML::Color(1, 1, 1, 1);
		this->camera[3] = this->camera[2] = this->camera[1] = this->camera[0];


		//キーボードの入力を受け取るオブジェクトを生成する
		XI::AnalogAxisKB	ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
		XI::AnalogAxisKB	rs = { DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD2 };
		XI::AnalogTriggerKB  tg = { DIK_E, DIK_R };
		XI::KeyDatas_KB	 key = {
			{ DIK_Z, XI::VGP::B1 },{ DIK_X, XI::VGP::B2 },{ DIK_C, XI::VGP::B3 },
			{ DIK_V, XI::VGP::B4 },{ DIK_A, XI::VGP::SE },{ DIK_S, XI::VGP::ST },
			{ DIK_Q, XI::VGP::L1 },{ DIK_W, XI::VGP::R1 },
			{ DIK_D, XI::VGP::L3 },{ DIK_NUMPAD5, XI::VGP::R3 },
		};
		//ゲームパッドパラメータ
		XI::KeyDatas_GP	but = {
			{ XI::RGP::B01, XI::VGP::B1 },{ XI::RGP::B02, XI::VGP::B2 },
			{ XI::RGP::B03, XI::VGP::B3 },{ XI::RGP::B04, XI::VGP::B4 },
			{ XI::RGP::B05, XI::VGP::L1 },{ XI::RGP::B06, XI::VGP::R1 },
			{ XI::RGP::B07, XI::VGP::SE },{ XI::RGP::B08, XI::VGP::ST },
			{ XI::RGP::B09, XI::VGP::L3 },{ XI::RGP::B10, XI::VGP::R3 },
		};
		this->in1 = XI::GamePad::CreateKB(ls, rs, tg, key);
		auto wgp = XI::GamePad::CreateGP(0, but);
		XI::GamePad::Link(this->in1, wgp);
		this->in2 = XI::GamePad::CreateGP(1, but);
		this->in3 = XI::GamePad::CreateGP(2, but);
		this->in4 = XI::GamePad::CreateGP(3, but);
		this->mouse = XI::Mouse::Create(ge->viewScaleW, ge->viewScaleH);

		//画面中央座標
		this->screenCenterPos.x = (float)this->screenWidth * 0.5f;
		this->screenCenterPos.y = (float)this->screenHeight * 0.5f;
		//ゲーム画面
		this->gameScreen.left = 0.0f;
		this->gameScreen.right = this->screenWidth;
		this->gameScreen.top = 0.0f;
		this->gameScreen.bottom = this->screenHeight;

		//上位スコアリセット
		for (int i = 0; i < size(topScores); ++i)
			topScores[i] = 0;

		//デバッグ用情報の表示ON/OFF
		DebugMode(false);

		//背景色の設定（デフォルト）
		this->dgi->EffectState().param.bgColor = ML::Color(0, 0.3f, 0.3f, 0.3f);

		playerPtr = player::Object::Create(true);
		playerPtr->SetPos(ML::Vec2(-500, -500));

		//初期実行タスク生成＆ゲームエンジンに登録
		auto  ft = TitleScene::Object::Create(true);

		//------------------------------------------------------------------------------------
		//レイヤー毎の描画のON/OFF
		//------------------------------------------------------------------------------------
		Layer_3D(false, false, false, false);
		Layer_2D(false, true);
		return true;
	}
	MyGameEngine::~MyGameEngine()
	{
		playerPtr->Kill();
		//小柳サウンドライブラリ
		se::Del();
		bgm::Del();
		this->FontImage.reset();

		free(c);

	}
	//ゲームエンジンに追加したもののステップ処理
	void MyGameEngine::UpDate()
	{
		//	入力装置の更新
		this->dii->UpDate();
		//	サウンドの更新(ストリーミング監視）
		this->dmi->UpDate();
		//小柳サウンドライブラリ更新
		se::EndCheck();
		bgm::EndCheck();

		//カウンタ動作
		ActCounter();

	}

	//2D描画環境のデフォルトパラメータ設定
	void MyGameEngine::Set2DRenderState(DWORD l_)
	{
		//共通？
		this->dgi->EffectState().RS_Def2D();
		this->dgi->EffectState().BS_Alpha();
		if (l_ == 0) {
		}
		if (l_ == 1) {
		}
	}
	//3D描画環境のデフォルトパラメータ設定
	void MyGameEngine::Set3DRenderState(DWORD l_)
	{
		//出力マージャを初期状態に戻す
		this->dgi->EffectState().BS_Std();
		//	カメラの設定更新
		camera[l_]->UpDate();
		//レイヤー0番に対する設定
		if (l_ == 0) {
		}
		if (l_ == 1) {
		}
		if (l_ == 2) {
		}
		if (l_ == 3) {
		}
	}
	//------------------------------------------------------------
	Camera::Camera(const ML::Vec3& tg_,	//	被写体の位置
		const ML::Vec3& pos_,	//	カメラの位置
		const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_)	//	アスペクト比（画面の比率に合わせる　横÷縦）	
		: pos(pos_), target(tg_), up(up_), fov(fov_), nearPlane(np_), forePlane(fp_), aspect(asp_)
	{
		UpDate();
	}
	Camera::~Camera()
	{
	}
	//	カメラを生成する
	Camera::SP Camera::Create(const ML::Vec3& tg_,	//	被写体の位置
		const ML::Vec3& pos_,	//	カメラの位置
		const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_)	//	アスペクト比（画面の比率に合わせる　横÷縦）	
	{
		return Camera::SP(new Camera(tg_, pos_, up_, fov_, np_, fp_, asp_));
	}
	//	カメラの設定
	void Camera::UpDate()
	{
		auto  dgi = DG::DGObject::GetInst();	if (dgi == 0) { return; }

		//ビュー行列を設定
		matView.LookAtLH(pos, target, up);
		//プロジェクション行列を設定
		matProj.PerspectiveFovLH(fov, aspect, nearPlane, forePlane);
		//カメラ関係の更新
		dgi->EffectState().param.matView = matView;
		dgi->EffectState().param.matProjection = matProj;
		dgi->EffectState().param.eyePos = pos;
	}

	//桁数を取得 0の時は1桁
	int MyGameEngine::GetDigit(int value)
	{
		if (value == 0)
			return 1;
		int barance = value;

		int digit;
		for (digit = 0; barance > 0; ++digit)
			barance *= 0.1f;

		return digit;
	}

	//フォントの画像で数字を表示
	void MyGameEngine::DrawNumFont(int value, ML::Vec2 drawPos, ML::Point size)
	{
		int digit = ge->GetDigit(value);
		int startPosX = drawPos.x + (digit * size.x);//表示開始位置
		//桁数分ループ
		for (int i = 0; i < digit; ++i) {
			int x = (value / (int)std::pow(10, i)) % 10 * ge->FontImageSize.x;
			int y = ge->FontImageSize.y;//最後の-2は2のべき乗のままだと下の文字が写るため微調整
			ML::Box2D src(x, 0, ge->FontImageSize.x, ge->FontImageSize.y);
			ML::Box2D draw(startPosX - (size.x * (i + 1)), drawPos.y, size.x, size.y);
			ge->FontImage->Draw(draw, src);
		}
	}

	//
	void MyGameEngine::DrawNumFont(int value, const FontImageInfo& imgInfo_, ML::Vec2 drawPos, ML::Point drawSize_)
	{
		int digit = ge->GetDigit(value);
		int startPosX = drawPos.x + (digit * drawSize_.x);//表示開始位置
		//桁数分ループ
		for (int i = 0; i < digit; ++i) {
			int x = (value / (int)std::pow(10, i)) % 10 * imgInfo_.size.x;
			int y = imgInfo_.size.y;
			ML::Box2D src(x, 0, imgInfo_.size.x, imgInfo_.size.y);
			ML::Box2D draw(startPosX - (drawSize_.x * (i + 1)), drawPos.y, drawSize_.x, drawSize_.y);
			imgInfo_.img->Draw(draw, src);
		}
	}
}

MyPG::MyGameEngine* ge;
