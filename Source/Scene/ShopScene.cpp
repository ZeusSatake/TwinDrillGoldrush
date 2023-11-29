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
		auto gotoBaseButton = SceneChangeButton::Object::Create(true);
		gotoBaseButton->SetEnterButton(XI::VGP::ST);
		gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoBaseButton->SetScene(this, Scene::Kind::Base);
		gotoBaseButton->SetText("_Φ");
		AddSceneChangeButton(gotoBaseButton);

		AddComponent(priceTag_Iron = make_shared<PriceTagComponent>(this));
		priceTag_Iron->Set("SzΞ", 15);

		AddComponent(priceTag_Bronze = make_shared<PriceTagComponent>(this));
		priceTag_Bronze->Set("ΊzΞ", 8);

		AddComponent(priceTag_Gold = make_shared<PriceTagComponent>(this));
		priceTag_Gold->Set("ΰzΞ", 100);

		player_ = player::Object::Create(true);
		player_->pos_ = ML::Vec2(-200, -200);
		//player_->wallet_->Recieve(150);

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
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "Vbv");
		//ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x, ge->screenCenterPos.y, 500, 500), "c" + to_string(player_->wallet_->GetBalance()));

		{
			string param = "";
			param += priceTag_Iron->GetName() + "F" + to_string(priceTag_Iron->CalcTotalPrice(1)) + "\n";
			param += priceTag_Bronze->GetName() + "F" + to_string(priceTag_Bronze->CalcTotalPrice(1)) + "\n";
			param += priceTag_Gold->GetName() + "F" + to_string(priceTag_Gold->CalcTotalPrice(1)) + "\n";
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 300, ge->screenCenterPos.y, 500, 500), param);
		}
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