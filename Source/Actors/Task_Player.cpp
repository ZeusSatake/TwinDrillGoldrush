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
	//\[XÌú»
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/prePlayer.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		this->playerImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();

		//f[^ú»
		this->box_->setHitBase(ML::Box2D{ -4,-8,8,16 });
		this->SetPos( ML::Vec2{ 50,480 });

		this->hp_->SetMaxHP(10,HP::MaxLifeSetMode::MaxHeal);
		//^XNÌ¶¬
		auto dl = drill::Object::Create(true);
		this->drill_ = dl;
		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú
		ge->KillAll_G("vC[");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ø«p¬^XNÌ¶¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{

		this->pState = this->state_->GetNowState();
		this->state_->moveCnt_++;
		this->Think();
		this->Move();
		drill_->SetPos(this->GetPos());
		drill_->dState = this->state_->GetNowState();
	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
		ML::Box2D pre = this->box_->getHitBase().OffsetCopy(this->GetPos());
		//vCLÌ`æ
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
			ML::Box2D src{ 0,0,32,64};
			//XN[Î
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->playerImg->Draw(draw, src);
			ge->debugFont->Draw(ML::Box2D(1000, 0, 500, 500), to_string(pre.x));
			ge->debugFont->Draw(ML::Box2D(100, 0, 1500, 500), "~»ÝÌìû@~Ú®:LXeBbN@pxÏX:RXeBbN@Wv:R1(WL[)@Ì@(Ì@[h):L1(QL[)@[hÏX:B2(XL[)@_bV:B1(ZL[)");
		}
		if (this->CheckHead())
			ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "ª»èI");
		if (this->CheckFoot())
			ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), "«»èI");
		ge->debugFont->Draw(ML::Box2D(1000, 400, 500, 500), to_string(this->GetMoveVec().x)+to_string(this->GetMoveVec().y));

		if (StateComponent::State::Jump == this->state_->GetNowState())
		{
			ge->debugFont->Draw(ML::Box2D(1000, 430, 500, 500), "WvI");
		}
		if (StateComponent::State::Drill == this->state_->GetNowState()||
			StateComponent::State::Mining == this->state_->GetNowState())
		{
			ge->debugFont->Draw(ML::Box2D(1000, 460, 500, 500), "Ì@[h");
		}
		else
		{
			ge->debugFont->Draw(ML::Box2D(1000, 490, 500, 500), "s®[h");
		}
	}
	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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
