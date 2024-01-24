//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "MartialFightScene.h"

#include "../../Camera.h"
#include "../EnemyClasses/BossLady.h"

#include "../System/Task_Save.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Enemys/Task_BlondeLady.h"
#include  "Task_Map.h"
#include  "Task_EnemyMap.h"
#include  "../Actors/UI/SceneChangeButton.h"

#include "../Components/SecondsTimerComponent.h"
#include "../Components/HPBarComponent.h"

#include "../Actors/Enemys/Task_LadyKumagai.h"
#include "../Actors/Enemys/Task_LadyHaraguchi.h"
#include "../Actors/Enemys/Task_LadyKiyohara.h"
#include "../Actors/Enemys/Task_LadyNegishi.h"
#include "../Actors/Enemys/Task_LadySatake.h"

#include "../Event/Task_EventEngine.h"

namespace MartialFightScene
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		AddComponent(transitionTimer_ = make_shared<SecondsTimerComponent>(this));
		transitionTimer_->SetCountSeconds(3.0f);

		clear_ = false;

		//★データ初期化
		ge->camera2D = ML::Box2D(0, 0, (int)ge->screenWidth, (int)ge->screenHeight);
		//ge->playerPtr->ResetState();
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(ML::Vec2(hpBar->GetSize().x * 0.5f, hpBar->GetSize().y * 0.5f));
		spawnableBoss_ = false;

		//★タスクの生成
		{
			if (auto ev = EventEngine::Object::Create_Mutex())
			{
				ev->Set("./data/event/eventmartialfightstart.txt");
			}
		}

		auto save = Save::Object::Create(true);
		nowStage_ = save->GetValue<int>(Save::Object::ValueKind::StageNo);

		{//武闘会
			auto map = Map::Object::Create(true);
			map->Load("MartialFight");
		}
		{//敵
			auto enemymap = EnemyMap::Object::Create(true);
			enemymap->Load("MartialFight1Enemy");
			enemymap->SetEnemy();
			enemyCount_ = ge->GetTasks<BlondeLady::Object>(BlondeLady::defGroupName, BlondeLady::defName)->size();
			//boss_ = ge->GetTask<Satake::Object>(Satake::defGroupName, Satake::defName);
		}
		{
			ge->playerPtr->SetPos(ML::Vec2{ 50,450 });
			auto camera = Camera::Object::Create(true);
			camera->horizontalScroll=true;
			camera->SetPos(ge->playerPtr->GetPos());
			camera->target = ge->playerPtr;
		}
		{//拠点に戻るボタン(デバッグ用
			auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("拠点へ");
			AddSceneChangeButton(gotoBaseButton);
		}

		//AddComponent(debugTimer = make_shared<SecondsTimerComponent>(this));
		//debugTimer->SetCountSeconds(0.3f);
		//debugTimer->Start();
		//debugMsg = "";

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (clear_)
		{
			auto save = Save::Object::Create(true);
			const int nowStage = save->GetValue<int>(Save::Object::ValueKind::StageNo);
			//次のステージへ
			if (nowStage < 3)
				save->SetValue(Save::Object::ValueKind::StageNo, nowStage + 1);
			save->Kill();
		}

		ge->KillAll_G("本編");
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_G("キャラクタ");
		ge->KillAll_G("敵");
		ge->KillAll_G("オブジェクト");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			CreateNextScene();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		Scene::UpDate();
		UpdateComponents();

		//{//デバッグ用
		//	debugMsg = spawnableBoss_ ?
		//		"ボスイベント開始フラグ　ON" :
		//		"ボスイベント開始フラグ　OFF";

		//	if (debugTimer->IsCountEndFrame() && enemyCount_ > 0)
		//	{
		//		auto enemy = ge->GetTask<BlondeLady::Object>(BlondeLady::defGroupName);
		//		enemy->Kill();
		//		debugTimer->Start();
		//	}

		//	auto inp = ge->in1->GetState();
		//	if (inp.ST.down)
		//		boss_.lock()->Kill();
		//}
		
		SpawnBoss();

		if (enemyCount_ <= 0 &&
			!boss_.lock()	 &&
			!clear_)
		{
			clearEvent_ = EventEngine::Object::Create_Mutex();
			clearEvent_.lock()->Set("./data/event/eventmartialfightclear.txt");
			clear_ = true;
		}

		if (clear_ && 
			!clearEvent_.lock())
		{
			CreateNextScene();
		}

		/*if (transitionTimer_->IsCountEndFrame())
		{
			this->Kill();
			return;
		}*/


	}

	void Object::SetBoss(const shared_ptr<BossLady>& boss)
	{
		boss_ = boss;
	}

	void Object::DecreaseEnemyCount()
	{
		if (enemyCount_ <= 0)
			return;

		--enemyCount_;

		//雑魚敵いなくなったらイベント開始
		if (enemyCount_ <= 0)
		{
			bossEvent_ = EventEngine::Object::Create_Mutex();
			//ev->Set("./data/event/");
			spawnableBoss_ = true;
		}
	}

	void Object::SpawnBoss()
	{
		if (!spawnableBoss_)
			return;

		//イベント再生中
		if (!EndOfSpawnBossEvent())
			return;

		shared_ptr<BossLady> boss;
		switch (nowStage_)
		{
		case 0:
			boss = Kumagai::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 1:
			boss = Haraguchi::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 2:
			boss = Kiyohara::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 3:
			boss = Negishi::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 4:
			boss = Satake::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		default:
			assert(!"設定されていない値です");
			break;
		}
		SetBoss(boss);
		spawnableBoss_ = false;
	}

	bool Object::EndOfSpawnBossEvent()
	{
		return enemyCount_ <= 0 &&
			   !bossEvent_.lock();
	}

	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "武闘会");

		if (clear_)
		{
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 50, 30, 500, 500), "ボスを倒したわね。", ML::Color(1, 1, 0, 0));
		}

		{//デバッグ用
			//ge->debugFont->Draw(ML::Box2D(100, 100, 500, 500), to_string(enemyCount_) + "\n" + debugMsg, ML::Color(1, 1, 0, 0));
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}