//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Drill.h"
#include "Task_Player.h"
#include "../../Source/Scene/Task_Map.h"

namespace  drill
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/preDrill.png");
		this->target = DG::Image::Create("./data/image/target.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->target.reset();
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
		this->box_->setHitBase(ML::Box2D{ -2,-2,4,4 });
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		this->SetAngle(this->UpdateDrillAngle());
		//this->SetPosX(this->GetPos().x+ cos(GetNowAngle()) * 15.f);
		//this->SetPosY(this->GetPos().y + sin(GetNowAngle()) * 15.f);
		this->SetMoveVec(ML::Vec2{ (cos(GetNowAngle()) * 16.f), (sin(GetNowAngle()) * 16.f) });
		this->SetDrawPos( this->GetPos() + this->GetMoveVec());
		this->DrillCheckMove(this->GetMoveVec());
		this->SearchBrocks();
		this->dState = this->state_->GetNowState();
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = ML::Box2D{-4,-4,8,8}.OffsetCopy(this->GetDrawPos());
		ML::Box2D src = ML::Box2D{ 0,0,64,64 };
		this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{4, 4});
		//XN[Ξ
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->res->img->Draw(draw, src);
		//----------------------------------------------------
		ML::Box2D tDraw = ML::Box2D{ (int)this->GetTargetPos().x*16,(int)this->GetTargetPos().y*16,16,16};
		tDraw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D tSrc = ML::Box2D{ 0,0,128,128 };
		this->res->target->Draw(tDraw, tSrc);
		//----------------------------------------------------
		ge->debugFont->Draw(ML::Box2D(1000, 100, 500, 500), "hΜpx:"+to_string(ML::ToDegree(GetNowAngle())));
		ML::Vec2 mapPoint{ (this->GetPos().x + 16 + ge->camera2D.x),(this->GetPos().y + 16 + ge->camera2D.x) };
		//ML::Box2D mapPoint{this->box_->getHitBase().OffsetCopy(this->GetPos()) };
		ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "hΜ}X:" + to_string((int)mapPoint.x/16)+" "+to_string((int)mapPoint.y/16));
		ge->debugFont->Draw(ML::Box2D(900, 200, 500, 500), to_string((int)GetTargetPos().x)+" "+ to_string((int)GetTargetPos().y));
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