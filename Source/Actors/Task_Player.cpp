//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Player.h"
#include  "Task_Player.h"
#include "Task_Drill.h"

namespace player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/prePlayer.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->playerImg.reset();
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
		this->box_->setHitBase(ML::Box2D{ -4,-8,8,16 });
		this->SetPos( ML::Vec2{ 50,480 });

		this->hp_->SetMaxHP(10,HP::MaxLifeSetMode::MaxHeal);
		//^XNΜΆ¬
		auto dl = drill::Object::Create(true);
		this->drill_ = dl;
		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->KillAll_G("vC[");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{

		this->pState = this->state_->GetNowState();
		this->moveCnt_++;
		this->Think();
		this->Move();
		drill_->SetPos(this->GetPos());
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D pre = this->box_->getHitBase().OffsetCopy(this->GetPos());
		//vCLΜ`ζ
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
			ML::Box2D src{ 0,0,32,64};
			//XN[Ξ
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->playerImg->Draw(draw, src);
			//ge->debugFont->Draw(ML::Box2D(1000, 0, 500, 500), to_string(pre.x));
		}
		/*if (this->CheckHead())
			ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "ͺ»θI");
		if (this->CheckFoot())
			ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), "«»θI");
		ge->debugFont->Draw(ML::Box2D(1000, 400, 500, 500), to_string(this->GetMoveVec().x)+to_string(this->GetMoveVec().y));

		if (StateComponent::State::Jump == this->state_->GetNowState())
		{
			ge->debugFont->Draw(ML::Box2D(1000, 430, 500, 500), "WvI");
		}*/
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
