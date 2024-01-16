//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Player.h"
#include  "Task_Player.h"
#include "Task_Drill.h"
#include "../Actors/UI/Task_DrawGauge.h"

namespace player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/prePlayer.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->playerImg.reset();
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
		this->box_->setHitBase(ML::Box2D{ -4,-8,8,16 });
		this->SetPos( ML::Vec2{ 0,0 });
		this->movement_->SetSpeed(2.f, 5.f, 0.f);
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, 10, 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32) * 10);

		//★タスクの生成
		auto dl = drill::Object::Create(true);
		this->drill_ = dl;

		//HPバー
		{
			hpGauge_ = DrawGauge::Object::Create(true);
			hpGauge_->Set(status_->HP.GetPercentage());
			hpGauge_->SetSupportScroll(false);
			ML::Point size{ 350, 60 };
			hpGauge_->SetDrawSize(size.x, size.y);
			hpGauge_->SetPos(size.x * 0.5f, ge->screenHeight - size.y * 0.5f);
			hpGauge_->SetVisible(false);
		}

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

		this->pState = this->state_->GetNowState();
		this->state_->moveCnt_++;
		this->Think();
		this->Move();
		drill_->SetPos(this->GetPos());
		drill_->dState = this->state_->GetNowState();

		hpGauge_->Set(status_->HP.GetPercentage());
		hpGauge_->SetVisible(pState != StateComponent::State::Non);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//プレイヤキャラの描画
		if(this->pState != StateComponent::State::Non)
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
			ML::Box2D src{ 0,0,32,64};
			//スクロール対応
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			if (this->pState != StateComponent::State::Dead)
			{
				if (this->unHitTimer_->IsCounting())
				{
					if (this->unHitTimer_->GetCount() % 2 == 0)
						this->res->playerImg->Draw(draw, src);

				}
				else
					this->res->playerImg->Draw(draw, src);
			}
		}
		DebugInfo();

	}
	void Object::DebugInfo()
	{
		//------------------------------------------------------------------
		string StateName;
		switch (pState)
		{
		case StateComponent::State::Non:
			StateName = "Non";
			break;
		case StateComponent::State::Idle:
			StateName = "Idle";
			break;
		case StateComponent::State::Walk:
			StateName = "Walk";
			break;
		case StateComponent::State::Attack:
			StateName = "Attack";
			break;
		case StateComponent::State::SpinAttack:
			StateName = "SpinAttack";
			break;
		case StateComponent::State::Damage:
			StateName = "Damage";
			break;
		case StateComponent::State::KnockBack:
			StateName = "KnockBack";
			break;
		case StateComponent::State::Dead:
			StateName = "Dead";
			break;
		case StateComponent::State::Jump:
			StateName = "Jump";
			break;
		case StateComponent::State::Fall:
			StateName = "Fall";
			break;
		case StateComponent::State::Dash:
			StateName = "Dash";
			break;
		case StateComponent::State::Drill:
			StateName = "Drill";
			break;
		case StateComponent::State::DrillDash:
			StateName = "DrillDash";
			break;
		case StateComponent::State::Mining:
			StateName = "Mining";
			break;
		case StateComponent::State::Appeal:
			StateName = "Appeal";
			break;

		}
		ge->debugFont->Draw(ML::Box2D{ 1000,0,500,500 }, "<NowState>" + StateName, ML::Color{1.f,1.f,0.f,0.f});
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1100,0,500,500 }, "<MoveCnt>" + to_string(this->state_->moveCnt_), ML::Color{1, 1, 0, 0});
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1000,20,500,500 }, "<PosX>" + to_string(this->GetPos().x) +" " + "<PosY>" + to_string(this->GetPos().y),ML::Color{1,1,0,0});
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1000,40,500,500 }, "Drill \: <PosX>" + to_string(this->drill_->GetPos().x) + " <PosY>" + to_string(this->drill_->GetPos().y), ML::Color{ 1,1,0,0 });
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1000,60,500,500 }, "HP\:" + to_string(this->status_->HP.GetNowHP()));
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
