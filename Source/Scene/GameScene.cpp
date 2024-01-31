//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "GameScene.h"

#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "EndingScene.h"
#include "../../Camera.h"

#include  "../Components/SecondsTimerComponent.h"
#include  "../Components/HPBarComponent.h"
#include  "../Components/GameOverEventComponent.h"

#include  "Task_EnemyMap.h"
#include  "Task_Map.h"
#include  "Task_JewelryMap.h"

#include  "../Actors/Task_MiningResult.h"
#include  "../Actors/Task_Player.h"
#include  "../Actors/Task_MiningResult.h"

#include  "../Actors/UI/SceneChangeButton.h"
#include  "../Actors/UI/Task_UIBanner.h"

#include  "../Actors/Enemys/Task_BlondeLady.h"

#include  "../System/Task_BackGround.h"
#include  "../System/Task_Save.h"

#include  "../Event/Task_EventEngine.h"


namespace  GameScene
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img[0] = DG::Image::Create("./data/image/ui/Bkani.png");
		this->img[1] = DG::Image::Create("./data/image/ui/LBkani.png");
		this->img[2] = DG::Image::Create("./data/image/ui/RBkani.png");
		this->img[3] = DG::Image::Create("./data/image/ui/LTkani.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 4; i++) {
			this->img[i].reset();
		}
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

		//★データ初期化
		this->render2D_Priority[1] = 0.0f;
		ge->debugRectLoad();
		bgm::LoadFile("game", "./data/sound/game.mp3");
		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->gameScreenWidth = ge->screenWidth;
		//ge->playerPtr->ResetState();
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(hpBar->GetSize().x * 0.5f, ge->screenHeight - hpBar->GetSize().y * 0.5f);
	
		fontImg.img = DG::Image::Create("./data/image/font_number.png");
		fontImg.size = ML::Point{ 20, 32 };
		ge->score = 0;
		ge->camera2D = ML::Box2D(0, 0, (int)ge->screenWidth, (int)ge->screenHeight);
		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		AddComponent(limitTimer_ = make_shared<SecondsTimerComponent>(this));
		limitTimer_->SetCountSeconds(60.0f * 3.0f);
		limitTimer_->Start();

		{//プレイヤ死亡時のイベント
			AddComponent(deathEvent_ = make_shared<GameOverEventComponent>(
				this,
				"./data/event/EventGameOverMining.txt",//ここでゲームオーバー時のイベントを設定
				0.8f));
			deathEvent_->SetPred(
				function<bool(void)>
				(
					[]()
					{
						return !ge->playerPtr->GetStatus()->HP.IsAlive();
					}
				)
			);
		}
		
		{//時間切れ時のイベント
			AddComponent(timeOverEvent_ = make_shared<GameOverEventComponent>(
				this,
				"./data/event/EventTimeOverMining.txt",//ここでゲームオーバー時のイベントを設定
				0.8f));
			timeOverEvent_->SetPred(
				function<bool(void)>
				(
					[this]()
					{
						return limitTimer_->WasCountEnd();
					}
				)
			);
		}

		auto save = Save::Object::Create(true);
		nowStage_ = save->GetValue<int>(Save::Object::ValueKind::StageNo);

		//★タスクの生成

		{
			//auto player = player::Object::Create(true);
			auto camera = Camera::Object::Create(true);
			camera->SetPos(ge->playerPtr->GetPos());
			camera->target = ge->playerPtr;
			//ge->playerPtr->SetPos(ML::Vec2{ 50,480 });
		}
		
		{//背景タスク生成
			ML::Point imgSize = { 2048, 1448 };
			ML::Point drawSize = { (int)ge->screenWidth, (int)ge->screenHeight };
			int sprit = 1;
			auto back = BackGround::Object::Create(true);
			back->SetUp("./data/image/gameback.png",
						imgSize,
						drawSize,
						BackGround::Object::RenderSize::FullScreen,
						sprit);
		}
		
		{//リザルト
			auto miningResult = MiningResult::Object::Create(true);
			miningResult->SetNowSecene(this);

			//間に合わせること優先の為決め打ちで設定 後で変更
			pair<Map::Object::ChipKind, int> targetOres[] =
			{
				make_pair(Map::Object::ChipKind::Damascus, 2),
				make_pair(Map::Object::ChipKind::Orichalcum, 3),
				make_pair(Map::Object::ChipKind::HihiIroKane, 8),
				make_pair(Map::Object::ChipKind::Adamantite, 3),
				make_pair(Map::Object::ChipKind::Ladyanite,1)
			};
			const auto& targetOre = targetOres[save->GetValue<int>(Save::Object::ValueKind::StageNo)];
			miningResult->SetTargetOre(targetOre.first, targetOre.second);
		}
		{//石 鉱石
			auto map = Map::Object::Create(true);
			map->Load("Map" + to_string(nowStage_ + 1) + "Stone");
		}
		{//宝石
			auto mapJewelry = JewelryMap::Object::Create(true);
			mapJewelry->Load("Map" + to_string(nowStage_ + 1) + "Jewelry");
		}
		{//敵
			auto enemymap = EnemyMap::Object::Create(true);
			enemymap->Load("Map" + to_string(nowStage_ + 1) + "Enemy");
			enemymap->SetEnemy();
		}

		{
			if (auto ev = EventEngine::Object::Create_Mutex())
			{
				ev->Set("./data/event/EventGameStart0" + to_string(nowStage_ + 1) + ".txt");
			}
		}

		{//UIバナー
			auto banner = UIBanner::Object::Create(true);
			banner->SetDrawSize(ML::Box2D(0, 0, 256, 256));
			banner->SetDrawArea(UIBanner::Object::DrawArea::LeftTop);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		ge->KillAll_G(Map::defGroupName);
		ge->KillAll_G(BackGround::defGroupName);
		ge->KillAll_GN(UIBanner::defGroupName, UIBanner::defName);
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(MiningResult::defGroupName, MiningResult::defName);
		ge->KillAll_G(BlondeLady::defGroupName);

		ge->debugRectReset();

		deathEvent_.reset();
		timeOverEvent_.reset();
		
		RemoveAllComponent();
		bgm::Stop("game");
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
		bgm::Play("game");
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//タイマー表示

	    ge->debugFont->Draw
		(
			ML::Box2D{ (int)ge->screenCenterPos.x - 20, 30, 500, 500 },
			to_string((int)limitTimer_->GetCount() / 60) + "\:" + to_string((int)limitTimer_->GetCount() % 60),
			ML::Color{ 1, 1, 0, 0 }
		);

		
		auto src = ML::Box2D(0, 0, 385, 52);
		for (int i = 0; i < 4; i++) {
			auto draw = ML::Box2D(20, 100 + 28 * i, 212, 28);
			this->res->img[i]->Draw(draw, src);
		}

		/*ge->debugFont->Draw
		(
			ML::Box2D(ge->screenCenterPos.x - 20, 30, 500, 500),
			to_string((int)limitTimer_->GetCount() / 60) + "：" + to_string((int)limitTimer_->GetCount() % 60),
			ML::Color(1, 1, 0, 0)
		);*/

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