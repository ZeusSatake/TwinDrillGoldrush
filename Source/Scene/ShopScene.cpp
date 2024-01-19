//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  <array>
#include  "../../MyPG.h"
#include  "ShopScene.h"
#include  "../Actors/UI/SceneChangeButton.h"

#include  "../Components/Money/WalletComponent.h"
#include  "../Components/Money/PriceTagComponent.h"

#include  "../Actors/UI/Task_BuyButton.h"
#include  "../System/Task_Save.h"

namespace ShopScene
{
	Resource::WP  Resource::instance;
	int Object::drill_MaxLevel = 5;
	int Object::defence_MaxLevel = 5;
	int Object::speed_MaxLevel = 5;
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
		ge->debugRectLoad();

		//★タスクの生成

		auto gotoBaseButton = SceneChangeButton::Object::Create(true);
		gotoBaseButton->SetEnterButton(XI::VGP::ST);
		gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoBaseButton->SetScene(this, Scene::Kind::Base);
		gotoBaseButton->SetText("拠点へ");
		AddSceneChangeButton(gotoBaseButton);

		save_ = Save::Object::Create(true);

		AddComponent(wallet = make_shared<WalletComponent>(this));
		wallet->RoadHaveMoney(save_);

		{//購入ボタン
			array<shared_ptr<BuyButton::Object>, 3> levelButtons;
			struct LevelButtonInfo
			{
				string name;
				int price;
				Save::Object::ValueKind kind;
				int max;
			};

			enum class levelButtonKind
			{
				Drill,
				Dress,
				Speed
			};

			LevelButtonInfo levelButtonInfos[levelButtons.size()] =
			{
				{
					"ドリル強化",
					125,
					Save::Object::ValueKind::DrillLevel,
					5
				},
				{
					"服",
					520,
					Save::Object::ValueKind::DefenceLevel,
					5
				},
				{
					"速度強化",
					330,
					Save::Object::ValueKind::SpeedLevel,
					5
				}
			};
			for (int i = 0; i < levelButtons.size(); ++i)
			{
				auto& button = levelButtons[i] = BuyButton::Object::Create(true);
				auto& info = levelButtonInfos[i];
				//button->SetPosX(300 + button->GetBox()->getHitBase().w * i);
				//button->SetPosY(ge->screenCenterPos.y);
				button->SetMouse(ge->mouse);
				button->SetEnterButton(XI::Mouse::MB::LB);
				button->SetResetTime(1.0f);
				button->SetBuyAmount(1);
				button->SetBuyerWallet(wallet);
				button->SetProduct
				(
					info.name,
					info.price,
					[this, button, info](void)
					{
						save_->SetValue(Save::Object::ValueKind::HaveMoney, button->GetBuyerWallet()->GetBalance());
						int nowLevel = save_->GetValue<int>(info.kind);
						if (nowLevel >= info.max)
						{
							button->SetEnable(false);
							return;
						}

						int nextLevel = nowLevel + 1;

						save_->SetValue(info.kind, nextLevel);

						//マックスになったときに入力受付を終了
						if (nextLevel >= drill_MaxLevel)
						{
							button->SetEnable(false);
							return;
						}
					}
				);

				//レベル最大のものは無効に
				if (save_->GetValue<int>(info.kind) >= info.max)
				{
					button->SetEnable(false);
				}
				buttons_.push_back(button);
			}

			//位置調整
			levelButtons.at((int)levelButtonKind::Drill)->SetPos(ML::Vec2(ge->screenCenterPos.x, 150));
			levelButtons.at((int)levelButtonKind::Dress)->SetPos(ML::Vec2(280, ge->screenCenterPos.y + 120));
			levelButtons.at((int)levelButtonKind::Speed)->SetPos(ML::Vec2(ge->screenWidth - 280, ge->screenCenterPos.y + 120));
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->debugRectReset();
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(BuyButton::defGroupName, BuyButton::defName);
		save_->Kill();

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

		for (const auto& button : buttons_)
		{
			ge->debugRect(button->GetBox()->getHitBase().OffsetCopy(button->GetPos()));
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ge->debugRectDraw();
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "ショップ");

		{
			string param =
				"プレイヤの所持金：" + to_string(wallet->GetBalance()) + "\n" + 
				"ドリル：" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DrillLevel)) + "\n" + 
				"防御　：" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DefenceLevel)) + "\n" + 
				"速度　：" + to_string(save_->GetValue<int>(Save::Object::ValueKind::SpeedLevel));
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 350, 200, 500, 500), param);
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