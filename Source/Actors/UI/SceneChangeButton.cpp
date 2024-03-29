//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "SceneChangeButton.h"

#include  "../../Scene/TitleScene.h"
#include  "../../Scene/GameScene.h"
#include  "../../Scene/ShopScene.h"
#include  "../../Scene/EndingScene.h"

namespace SceneChangeButton
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
		box_->setHitBase(ML::Box2D(-100, -50, 200, 100));
		render2D_Priority[1] = 0.1f;
		
		SetEnterButton(XI::VGP::B1);
		SetRecieveInputEnable(true);
		SetSelected(false);
		SetMouse(ge->mouse);
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		image_.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		ToggleButton::UpDate();
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//Drawtext(ge->debugFont, false);

		if (image_ != nullptr)
		{
			const ML::Box2D& draw(box_->getHitBase());
			const ML::Box2D	 src(0, 0, draw.w, draw.h);
			image_->Draw(draw.OffsetCopy(GetPos()), src);

			//auto oDraw = draw.OffsetCopy(GetPos());
			//ge->debugFont->Draw(ML::Box2D(200, 200, 500, 500),
			//	"draw x:" + to_string(oDraw.x) + " y:" + to_string(oDraw.y) + 
			//	" w:" + to_string(oDraw.w) + " h:" + to_string(oDraw.h));
		}
	}
	void Object::OnEvent()
	{
		nowScene_->SetNextScene(nextScene_);
	}
	void Object::OffEvent()
	{
	}
	void Object::Set(const string& text, Scene* nowScene, const Scene::Kind nextScene, const XI::Mouse::MB mouseEnterButton, const XI::VGP gamePadEnterButton, const Actor* selectorPtr)
	{
		SetText(text);
		SetScene(nowScene, nextScene);
		SetEnterButton(mouseEnterButton);
		SetEnterButton(gamePadEnterButton);
		if (selectorPtr != nullptr)
			SetSelector(selectorPtr);
	}
	void Object::Set(const SetInfo& setInfo)
	{
		Set(
			setInfo.text + "Φ",
			setInfo.nowScene,
			setInfo.nextScene,
			setInfo.mouseEnterButton,
			setInfo.gamePadEnterButton,
			setInfo.selectorPtr
		);
	}
	void Object::SetScene(Scene* nowScene, const Scene::Kind& nextScene)
	{
		nowScene_ = nowScene;
		nextScene_ = nextScene;
	}

	void Object::SetImage(const string& path)
	{
		if (image_ == nullptr)
			image_ = DG::Image::Create(path);
		else
			image_->ReLoad(path);
	}

	void Object::SetSize(const ML::Point& size)
	{
		box_->setHitBase(ML::Box2D(size.x * -0.5f, size.y * -0.5f, size.x, size.y));
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