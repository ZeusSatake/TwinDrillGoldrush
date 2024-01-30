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
		this->img = DG::Image::Create("./data/image/DrillMap.png");
		this->target = DG::Image::Create("./data/image/target.png");
		this->debug = DG::Image::Create("./data/image/shot.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->target.reset();
		this->debug.reset();
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
		this->setAnim();
		this->box_->setHitBase(ML::Box2D{ -2,-2,4,4 });
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		RemoveAllComponent();
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->plPos = ge->playerPtr->GetPos();
		this->SetAngle(this->UpdateDrillAngle());
		this->SetMoveVec(ML::Vec2{ (cos(GetNowAngle()) * 16.f), (sin(GetNowAngle()) * 16.f) });
		this->SetDrawPos( this->GetPos() + this->GetMoveVec()+this->GetAnimMove());//この時点で見かけ上のドリルの描画を決定する
		
		
		this->DrillCheckMove(this->GetMoveVec()*this->GetLenght());//ターゲット用の矩形判定

		this->dState = this->state_->GetNowState();
		this->UpdateTargetPos(this->ChangeBrockPos());
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if(this->GetMode()!=Mode::Non )
		{
			//ML::Box2D draw = ML::Box2D{ -4,-4,8,8 }.OffsetCopy(this->GetDrawPos());
			//ML::Box2D src = ML::Box2D{ 0,0,64,64 };
			//this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{ 4, 4 });
			////スクロール対応
			//draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//this->res->img->Draw(draw, src);
			 AnimInfo animInfo = this->animManager_->Play((int)this->GetMode());
			ML::Box2D Predraw = animInfo.GetDraw();
			
			ML::Box2D draw = Predraw.OffsetCopy(this->GetPos());//※座標は指定する必要あり
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{ 4, 4 });
			this->res->img->Draw(draw, animInfo.GetSrc());
			//----------------------------------------------------
			ML::Box2D tDraw = ML::Box2D{ (int)this->GetTargetPos().x * 16,(int)this->GetTargetPos().y * 16,16,16 };
			tDraw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			ML::Box2D tSrc = ML::Box2D{ 0,0,128,128 };
			if (this->GetMode() == Drill::Mode::Drill)
				this->res->target->Draw(tDraw, tSrc);
		}
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