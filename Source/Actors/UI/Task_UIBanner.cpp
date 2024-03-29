//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_UIBanner.h"

#include  "../../Components/HPBarComponent.h"
#include  "../../Actors/Task_Player.h"

namespace UIBanner
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		backImage = DG::Image::Create("./data/image/backGround/uiBanner/Newstuff 1.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		backImage.reset();
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
		render2D_Priority[1] = 0.7f;
		SetBackGroundAlpha(1.0f);
		SetMargin(37);
		
		//HPo[\¦Κu
		NormalizeHPBarPos();
		ge->playerPtr->GetHPBar()->SetSupportScroll(false);
		
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

	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D src(40, 112, 192, 192);
		res->backImage->Draw(drawSize_.OffsetCopy(GetPos()), src, ML::Color(alpha_, 1, 1, 1));
	}

	void Object::NormalizeHPBarPos()
	{
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(ML::Vec2(hpBar->GetSize().x * 0.5f + GetPos().x + margin_, hpBar->GetSize().y * 0.5f + GetPos().y + margin_));
	}

	void Object::SetDrawSize(const ML::Box2D& drawSize)
	{
		drawSize_ = drawSize;
		ge->playerPtr->GetHPBar()->SetDrawSize(drawSize_.w * 0.8f, drawSize_.h * 0.2f);
	}

	void Object::SetBackGroundAlpha(const float alpha)
	{
		alpha_ = alpha;
	}

	void Object::SetMargin(const float margin)
	{
		margin_ = margin;
		NormalizeHPBarPos();
	}
	
	void Object::SetDrawArea(DrawArea drawArea)
	{
		switch (drawArea)
		{
		case DrawArea::LeftTop:
			SetPos(ML::Vec2(0, 0));
			break;

		case DrawArea::RightTop:
			SetPos(ML::Vec2(ge->screenWidth - drawSize_.w, 0));
			break;

		case DrawArea::RightBottom:
			SetPos(ML::Vec2(ge->screenWidth - drawSize_.w, ge->screenHeight - drawSize_.h));
			break;

		case DrawArea::LeftBottom:
			SetPos(ML::Vec2(0, ge->screenHeight - drawSize_.h));
			break;
		}

		NormalizeHPBarPos();
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