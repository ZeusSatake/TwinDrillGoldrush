//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_EtoHaiji.h"
#include  "../../Actors/Task_Player.h"

namespace EtoHaiji
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/EtoHaiji.png");
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
		status_->HP.Initialize(20);
		status_->speed.Initialize(2.f, 2.f, 10.f);
		status_->attack.Initialize(25, 50);
		status_->defence.Initialize(0, 100);
		box_->setHitBase(ML::Box2D{ -8,-16,16,32 });
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, status_->speed.GetFallSpeed(), 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32)*20);

		angle_LR_ = Angle_LR::Right;

		SetNowState(Enemy::Patrol);

		SetFov(200.f);
		unHitTimer_->SetCountFrame(60);

		SetTarget(ge->playerPtr.get());
		this->render2D_Priority[1] = 0.2f;
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		RemoveAllComponent();
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
		Enemy::UpDate();
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = box_->getHitBase();
			ML::Box2D src = ML::Box2D(0, 0, 48, 64);
			if (angle_LR_ == Angle_LR::Right)
			{
				draw.x = -draw.x;
				draw.w = -draw.w;
			}
			//XN[Ξ
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			draw.Offset(GetPos());
			res->img->Draw(draw, src);
		}
	}
	//-------------------------------------------------------------------
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