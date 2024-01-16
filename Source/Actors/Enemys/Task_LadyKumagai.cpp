//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_LadyKumagai.h"
#include "../Task_Player.h"

namespace Kumagai
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/LadySatake.png");
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
		status_->HP.Initialize(150);
		status_->attack.Initialize(15, 100);
		status_->defence.Initialize(0, 100);
		status_->speed.Initialize(2.5f, 100.f, 10.f);
		box_->setHitBase(ML::Box2D{ -8,-16,16,32 });
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, status_->speed.GetFallSpeed(), 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32) * 10);

		angle_LR_ = Angle_LR::Left;

		SetPreState(AIState::Idle);
		SetNowState(AIState::Idle);

		SetFov(1000.f);

		moveCnt_->SetCountFrame(0);
		unHitTimer_->SetCountFrame(15);

		SetTarget(ge->playerPtr.get());
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = box_->getHitBase().OffsetCopy(GetPos());
			ML::Box2D src = ML::Box2D(0, 0, 500, 615);
			//XN[Ξ
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

			res->img->Draw(draw, src);
		}

		ge->debugFont->Draw(ML::Box2D(1000, 300, 700, 700), to_string(GetStatus()->HP.GetNowHP()));
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