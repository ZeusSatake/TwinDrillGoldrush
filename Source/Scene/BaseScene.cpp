//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "BaseScene.h"

#include  "../Actors/UI/SceneChangeButton.h"

namespace BaseScene
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
		
		//★タスクの生成
		auto gotoTitleButton = SceneChangeButton::Object::Create(true);
		gotoTitleButton->SetEnterButton(XI::VGP::ST);
		gotoTitleButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoTitleButton->SetScene(this, Scene::Kind::Title);
		gotoTitleButton->SetText("タイトルへ");
		AddSceneChangeButton(gotoTitleButton);

		auto gotoShopSceneButton = SceneChangeButton::Object::Create(true);
		gotoShopSceneButton->SetScene(this, Scene::Kind::Shop);
		gotoShopSceneButton->SetEnterButton(XI::VGP::B1);
		gotoShopSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoShopSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x - 200, ge->screenCenterPos.y);
		gotoShopSceneButton->SetText("ショップへ");
		AddSceneChangeButton(gotoShopSceneButton);

		auto gotoMartialFightSceneButton = SceneChangeButton::Object::Create(true);
		gotoMartialFightSceneButton->SetScene(this, Scene::Kind::MartialFight);
		gotoMartialFightSceneButton->SetEnterButton(XI::VGP::B2);
		gotoMartialFightSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoMartialFightSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y);
		gotoMartialFightSceneButton->SetText("武闘会へ");
		AddSceneChangeButton(gotoMartialFightSceneButton);

		auto gotoMiningSceneButton = SceneChangeButton::Object::Create(true);
		gotoMiningSceneButton->SetScene(this, Scene::Kind::Mining);
		gotoMiningSceneButton->SetEnterButton(XI::VGP::B3);
		gotoMiningSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoMiningSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x + 200, ge->screenCenterPos.y);
		gotoMiningSceneButton->SetText("採掘場へ");
		AddSceneChangeButton(gotoMiningSceneButton);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G(SceneChangeButton::defGroupName);

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "拠点");
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