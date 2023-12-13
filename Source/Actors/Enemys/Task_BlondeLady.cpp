//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_BlondeLady.h"
#include  "../../Actors/Task_Player.h"

namespace BlondeLady
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/BlondeLady.png");
		fanImg = DG::Image::Create("./data/image/Slash.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		img.reset();
		fanImg.reset();
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
		box_->setHitBase(ML::Box2D{ -8,-16,16,32 });
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, 10, 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32)*10);

		angle_LR_ = Angle_LR::Right;

		SetPreState(AIState::Approach);
		SetNowState(AIState::Approach);

		SetFov(200.f);
		SetRange(10.f);

		moveCnt_->SetCountFrame(90);
		fanEdge_->setHitBase(ML::Box2D{ -4,-8,8,32 });

		SetTarget(ge->playerPtr.get());
		//★タスクの生成
		return  true;
	}

	void BeginPlay()
	{

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
		/*auto pl = ge->GetTask<player::Object>(player::defGroupName, player::defName);
		SetTarget(pl.get());*/

		moveCnt_->Update();
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = box_->getHitBase().OffsetCopy(GetPos());
			ML::Box2D src = ML::Box2D(0, 0, 500, 615);
			//スクロール対応
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

			res->img->Draw(draw, src);
		}
		{
			ML::Box2D draw = fanEdge_->getHitBase().OffsetCopy(ML::Vec2(GetPos().x + 16, GetPos().y));
			ML::Box2D src = ML::Box2D(0, 0, 16, 64);

			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			res->fanImg->Draw(draw, src);
		}

		if (IsAttacking())
		{
			ge->debugFont->Draw(ML::Box2D(500, 700, 500, 500), "攻撃中");
		}
		

		string stateName;
		switch (GetNowState())
		{
		case AIState::Idle:
			stateName = "待機";
			break;
		case AIState::Approach:
			stateName = "接近";
			break;
		case AIState::Attack:
			stateName = "攻撃";
			break;
		}
		ge->debugFont->Draw(ML::Box2D(1000, 700, 500, 500), stateName);
		auto pl = ge->GetTask<player::Object>(player::defGroupName, player::defName);
		SetTarget(pl.get());

		if(WithinRange(GetTarget()))
		ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), "攻撃範囲");

		

		ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), to_string(moveCnt_->GetCount()));
	}
	//-------------------------------------------------------------------
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