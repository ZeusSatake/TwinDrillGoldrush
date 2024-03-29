//-------------------------------------------------------------------
//UIJ[\
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_Cursor.h"
#include  "../../Components/Movement.h"
#include  "../../UIBase/ToggleButton.h"

namespace Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		image_ = DG::Image::Create("./data/image/ui/cursor/preDrill.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		image_.reset();
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
		this->SetPos(ge->screenCenterPos);

		AddComponent(movement_ = make_shared<Movement>(this));
		//p[^Μέθ
		movement_->SetSpeed(8.0f, 15.0f, 0.5f);
		movement_->SetDecelerationRate(ML::Percentage(30.0f));
		movement_->SetAcceleration(20.0f);

		box_->setHitBase(ML::Box2D(-28, -28, 56, 56));
		
		auto buttons = ge->GetTasks<ToggleButton>("UI", "Button");
		for (auto& button : *buttons)
		{
			button->SetSelector(this);
		}
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		movement_.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		movement_->LStickInputToMove(ge->in1);
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D src(24, 24, 64, 64);
		res->image_->Draw(box_->getHitBase().OffsetCopy(this->GetPos()), src);
	}

	void Object::SetEnterButton(const XI::VGP enterButton)
	{
		enterButton_ = enterButton;
	}
	XI::VGP Object::GetEnterButton() const
	{
		return enterButton_;
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