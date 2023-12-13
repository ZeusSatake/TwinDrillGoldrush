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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/preDrill.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->box_->setHitBase(ML::Box2D{ -4,-4,8,8 });
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
		this->SetAngle(this->UpdateDrillAngle());
		//this->SetPosX(this->GetPos().x+ cos(GetNowAngle()) * 15.f);
		//this->SetPosY(this->GetPos().y + sin(GetNowAngle()) * 15.f);
		this->SetMoveVec(ML::Vec2{ (cos(GetNowAngle()) * 15.f), (sin(GetNowAngle()) * 15.f) });
		this->DrillCheckMove(this->GetMoveVec());
		this->dState = this->state_->GetNowState();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
		ML::Box2D src = ML::Box2D{ 0,0,64,64 };
		this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{4, 4});
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->res->img->Draw(draw, src);
		//----------------------------------------------------
		ge->debugFont->Draw(ML::Box2D(1000, 100, 500, 500), "ドリルの角度:"+to_string(ML::ToDegree(GetNowAngle())));
		ML::Vec2 mapPoint{ (this->GetPos().x+16 + ge->camera2D.x),(this->GetPos().y+16 + ge->camera2D.x )};
		//ML::Box2D mapPoint{this->box_->getHitBase().OffsetCopy(this->GetPos()) };
		ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "ドリルのマス:" + to_string((int)mapPoint.x/16)+" "+to_string((int)mapPoint.y/16));
		
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