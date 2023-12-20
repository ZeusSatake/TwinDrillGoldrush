//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "GameScene.h"

#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "EndingScene.h"

#include  "../System/Task_BackGround.h"
#include  "Task_Map.h"
#include  "Task_JewelryMap.h"
#include  "../Actors/Task_MiningResult.h"

#include  "Task_EnemyMap.h"

#include  "../Actors/UI/SceneChangeButton.h"

#include "../Actors/Task_Player.h"
#include "../../Camera.h"
#include "../System/Task_Save.h"

namespace  GameScene
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

		//★データ初期化
		this->render2D_Priority[1] = 0.0f;
		ge->debugRectLoad();

		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->gameScreenWidth = ge->screenWidth;
		
		fontImg.img = DG::Image::Create("./data/image/font_number.png");
		fontImg.size = ML::Point{ 20, 32 };
		ge->score = 0;
		ge->camera2D = ML::Box2D(0, 0, (int)ge->screenWidth, (int)ge->screenHeight);
		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		auto save = Save::Object::Create(true);

		//★タスクの生成
		{
			//auto player = player::Object::Create(true);
			auto camera = Camera::Object::Create(true);
			camera->SetPos(ge->playerPtr->GetPos());
			camera->target = ge->playerPtr;
			ge->playerPtr->SetPos(ML::Vec2{ 50,480 });
		}
		
		{//背景タスク生成
			ML::Point imgSize = { 960, 500 };
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
				make_pair(Map::Object::ChipKind::Orichalcum, 1),
				make_pair(Map::Object::ChipKind::HihiIroKane, 1),
				make_pair(Map::Object::ChipKind::Adamantite, 1)
			};
			const auto& targetOre = targetOres[save->GetValue<int>(Save::Object::ValueKind::StageNo)];
			miningResult->SetTargetOre(targetOre.first, targetOre.second);
		}
		{//石 鉱石
			auto map = Map::Object::Create(true);
			map->Load("MapStone");
		}
		{//宝石
			auto mapJewelry = JewelryMap::Object::Create(true);
			mapJewelry->Load("MapJewelry");
		}
		{//敵
			auto enemymap = EnemyMap::Object::Create(true);
			enemymap->Load("MapEnemy");
			enemymap->SetEnemy();
		}

		{//拠点に戻るボタン(デバッグ用
			auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("拠点へ");
			AddSceneChangeButton(gotoBaseButton);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		ge->KillAll_G("本編");
		ge->KillAll_G("システム");
		ge->KillAll_G(SceneChangeButton::defGroupName);
		ge->KillAll_G("キャラクタ");
		ge->KillAll_G("敵");

		ge->debugRectReset();

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

		auto inp = ge->in1->GetState();
		if (inp.SE.down) {
			this->Kill();
		}
		if (ge->GameOverFlag) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
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