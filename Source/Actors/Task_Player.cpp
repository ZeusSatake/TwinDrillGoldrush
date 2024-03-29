//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Player.h"
#include  "Task_Player.h"
#include "Task_Drill.h"
#include "../Actors/UI/Task_DrawGauge.h"
#include "../Components/HPBarComponent.h"

namespace player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/PlayerMap.png");
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

		this->SetAnim();

		//★データ初期化
		this->render2D_Priority[0] = 0.2f;
		this->SetPos( ML::Vec2{ 0,0 });
		this->movement_->SetSpeed(2.f, 2.f, 0.f);
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, 10, 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32) * 10);

		//★タスクの生成
		auto dl = drill::Object::Create(true);
		this->drill_ = dl;

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

		hpBar_->Update();
		this->hpBar_->SetVisible(pState != StateComponent::State::Non);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//プレイヤキャラの描画
		if(this->pState != StateComponent::State::Non)
		{
			AnimInfo animInfo = this->animManager_->Play((int)this->state_->GetNowState());
			ML::Box2D Predraw = animInfo.GetDraw();
			if (this->angle_LR_ == Angle_LR::Right)
			{
				Predraw.x = -Predraw.x;
				Predraw.w = -Predraw.w;
			}
			ML::Box2D draw = Predraw.OffsetCopy(this->GetPos());//※座標は指定する必要あり
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->playerImg->Draw(draw, animInfo.GetSrc());
		}
		//DebugInfo();

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
		ge->debugFont->Draw(ML::Box2D{ 1000,40,500,500 }, "Drill : <PosX>" + to_string(this->drill_->GetPos().x) + " <PosY>" + to_string(this->drill_->GetPos().y), ML::Color{ 1,1,0,0 });
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1000,60,500,500 }, "HP:" + to_string(this->status_->HP.GetNowHP()));
		//------------------------------------------------------------------
		ge->debugFont->Draw(ML::Box2D{ 1000,80,500,500 }, "UnHitTimer:" + to_string(this->unHitTimer_->GetCount()));
		ge->debugFont->Draw(ML::Box2D{ 1000,100,500,500 }, "countdown:" + to_string(this->cooldown->GetCount()));
		ge->debugFont->Draw(ML::Box2D{ 1000,120,500,500 }, "overheat:" + to_string(this->overheat->GetCount()));
		ge->debugFont->Draw(ML::Box2D{ 1000,140,500,500 }, "durability:" + to_string(this->drill_->GetDurability())+" "+to_string(this->drill_->GetMaxDurability()));
		ge->debugFont->Draw(ML::Box2D{ 1000,160,500,500 }, "external" + to_string(this->GetMoveVec().x) + " " + to_string(this->GetMoveVec().y));
	}

	void Object::SceneTransitionInitialize(Scene::Kind nextScene)
	{
		switch (nextScene)
		{
			case Scene::Base:
				SetPos(ML::Vec2(-500, -500));
				status_->HP.TakeHeal(status_->HP.GetMaxHP());
				HiddenPlayer();
			break;

			case Scene::Mining:
				SetPos(ML::Vec2(50, 480));
				ResetState();
			break;

			case Scene::MartialFight:
				SetPos(ML::Vec2(50, 480));
				ResetState();
			break;

			case Scene::Shop:
				SetPos(ML::Vec2(-500, -500));
				HiddenPlayer();
			break;

			case Scene::Ending:
				SetPos(ML::Vec2(-500, -500));
				HiddenPlayer();
			break;

			case Scene::Opening:
				SetPos(ML::Vec2(-500, -500));
				HiddenPlayer();
			break;
		}
		this->UpdateStates();
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
