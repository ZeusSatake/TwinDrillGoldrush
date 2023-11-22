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
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		
		//^XNΜΆ¬
		auto gotoShopSceneButton = SceneChangeButton::Object::Create(true);
		gotoShopSceneButton->SetScene(this, Scene::Kind::Shop);
		gotoShopSceneButton->SetEnterButton(XI::VGP::B1);
		gotoShopSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoShopSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x - 200, ge->screenCenterPos.y);
		gotoShopSceneButton->SetText("VbvΦ");
		AddSceneChangeButton(gotoShopSceneButton);

		auto gotoMartialFightSceneButton = SceneChangeButton::Object::Create(true);
		gotoMartialFightSceneButton->SetScene(this, Scene::Kind::MartialFight);
		gotoMartialFightSceneButton->SetEnterButton(XI::VGP::B2);
		gotoMartialFightSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoMartialFightSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y);
		gotoMartialFightSceneButton->SetText("¬οΦ");
		AddSceneChangeButton(gotoMartialFightSceneButton);

		auto gotoMiningSceneButton = SceneChangeButton::Object::Create(true);
		gotoMiningSceneButton->SetScene(this, Scene::Kind::Mining);
		gotoMiningSceneButton->SetEnterButton(XI::VGP::B3);
		gotoMiningSceneButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoMiningSceneButton->pos_ = ML::Vec2(ge->screenCenterPos.x + 200, ge->screenCenterPos.y);
		gotoMiningSceneButton->SetText("Μ@κΦ");
		AddSceneChangeButton(gotoMiningSceneButton);

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->KillAll_G(SceneChangeButton::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
			CreateNextScene();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		Scene::UpDate();
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "_");
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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