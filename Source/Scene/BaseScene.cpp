//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "BaseScene.h"
#include  <array>

#include  "../Actors/UI/SceneChangeButton.h"
#include  "../Actors/UI/Task_Cursor.h"
#include  "../System/Task_Save.h"
#include  "../Event/Task_EventEngine.h"

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
		ge->debugRectLoad();
		render2D_Priority[1] = 0.0f;

		//★タスクの生成

		{//シーン遷移ボタン, カーソル作成
			auto cursor = Cursor::Object::Create(true);
			cursor->SetPos(ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y + 80));
			cursor->SetEnterButton(XI::VGP::ST);

			XI::Mouse::MB mouseEnterButton = XI::Mouse::MB::LB;
			XI::VGP		  gamePadEnterButton = cursor->GetEnterButton();

			enum class ButtonKind
			{
				GoTitle,
				GoShop,
				GoMartialFight,
				GoMining
			};
			std::array<SceneChangeButton::Object::SetInfo, 4> buttonInfos =
			{
				SceneChangeButton::Object::SetInfo{
					"タイトル",
					this,
					Scene::Kind::Title,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"ショップ",
					this,
					Scene::Kind::Shop,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"闘技場",
					this,
					Scene::Kind::MartialFight,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"採掘場",
					this,
					Scene::Kind::Mining,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				}
			};

			array<SceneChangeButton::Object::SP, buttonInfos.size()> buttons;
			for (int i = 0; i < buttonInfos.size(); ++i)
			{
				const auto& buttonInfo = buttonInfos.at(i);

				auto button = buttons.at(i) = SceneChangeButton::Object::Create(true);
				button->Set(buttonInfo);

				if (i > 0)
				{
					button->SetSize(ML::Point{ 200, 100 });
					button->SetImage("./data/image/ui/" + buttonInfo.text + "遷移.png");
				}

				int buttonMargin = 10;
				float startX = ge->screenCenterPos.x - (buttonInfos.size() * 0.5f) * (button->GetBox()->getHitBase().w + buttonMargin) + button->GetBox()->getHitBase().w * 0.5f;
				button->SetPos(
					ML::Vec2(
						startX + ((button->GetBox()->getHitBase().w + buttonMargin) * i),
						ge->screenCenterPos.y));

				AddSceneChangeButton(button);
				buttons_.push_back(button);
			}

			const auto& goTitleButtonSize = buttons.at((int)ButtonKind::GoTitle)->GetBox()->getHitBase();
			buttons.at((int)ButtonKind::GoTitle)->SetPos(ML::Vec2(goTitleButtonSize.w * 0.5f, goTitleButtonSize.h * 0.5f));

			//buttons.at((int)ButtonKind::GoShop)->SetPos(ML::Vec2());
			//buttons.at((int)ButtonKind::GoTitle)->SetPos(ML::Vec2());
			//buttons.at((int)ButtonKind::GoTitle)->SetPos(ML::Vec2());
		}

		auto save = Save::Object::Create(true);
		if (save->GetValue<int>(Save::Object::ValueKind::EndOfBaseTutorial) == 0)
		{
			auto ev = EventEngine::Object::Create_Mutex();
			ev->Set("./data/event/EventBaseTutorial.txt");
			save->SetValue(Save::Object::ValueKind::EndOfBaseTutorial, 1.0f);
		}
		save->Kill();

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(Cursor::defGroupName, Cursor::defName);
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

		for (auto& button : buttons_)
		{
			ge->debugRect(button->GetBox()->getHitBase().OffsetCopy(button->GetPos()));
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "拠点");
		ge->debugRectDraw();
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