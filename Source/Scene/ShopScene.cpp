//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "ShopScene.h"
#include  "../Actors/UI/SceneChangeButton.h"
#include  "../Actors/Task_Player.h"
#include  "../Components/Money/WalletComponent.h"
#include  "../Components/Money/PriceTagComponent.h"

namespace ShopScene
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
		auto gotoBaseButton = SceneChangeButton::Object::Create(true);
		gotoBaseButton->SetEnterButton(XI::VGP::ST);
		gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoBaseButton->SetScene(this, Scene::Kind::Base);
		gotoBaseButton->SetText("拠点へ");
		AddSceneChangeButton(gotoBaseButton);

		AddComponent(priceTag_Iron = make_shared<PriceTagComponent>(this));
		priceTag_Iron->Set("鉄鉱石", 15);

		AddComponent(priceTag_Bronze = make_shared<PriceTagComponent>(this));
		priceTag_Bronze->Set("銅鉱石", 8);

		AddComponent(priceTag_Gold = make_shared<PriceTagComponent>(this));
		priceTag_Gold->Set("金鉱石", 100);

		player_ = player::Object::Create(true);
		player_->pos_ = ML::Vec2(-200, -200);
		//player_->wallet_->Recieve(150);

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
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "ショップ");
		//ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x, ge->screenCenterPos.y, 500, 500), "残高" + to_string(player_->wallet_->GetBalance()));

		{
			string param = "";
			param += priceTag_Iron->GetName() + "：" + to_string(priceTag_Iron->CalcTotalPrice(1)) + "\n";
			param += priceTag_Bronze->GetName() + "：" + to_string(priceTag_Bronze->CalcTotalPrice(1)) + "\n";
			param += priceTag_Gold->GetName() + "：" + to_string(priceTag_Gold->CalcTotalPrice(1)) + "\n";
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 300, ge->screenCenterPos.y, 500, 500), param);
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