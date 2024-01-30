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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		backImage = DG::Image::Create("./data/image/backGround/uiBanner/Newstuff 1.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		backImage.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		render2D_Priority[1] = 0.7f;
		SetBackGroundAlpha(1.0f);
		SetMargin(37);
		
		//HPバー表示位置
		NormalizeHPBarPos();
		ge->playerPtr->GetHPBar()->SetSupportScroll(false);
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
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

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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