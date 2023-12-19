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

		status_->HP.Initialize(100);
		this->hp_->SetMaxHP(10,HP::MaxLifeSetMode::MaxHeal);
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
		//ge->KillAll_G("プレイヤー");

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//プレイヤキャラの描画
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
			ML::Box2D src{ 0,0,32,64};
			//スクロール対応
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->playerImg->Draw(draw, src);
		}
		this->DebugInfo();
	}

	void Object::DebugInfo()
	{
		ge->debugFont->Draw(ML::Box2D(1000, 0, 500, 500), "現在位置:"+to_string(this->GetPos().x) + " " + to_string(this->GetPos().y));
		ge->debugFont->Draw(ML::Box2D(100, 0, 1500, 500), "~現在の操作方法~移動:Lスティック　角度変更:Rスティック　ジャンプ:R1(Wキー)　採掘(採掘モード中):L1(Qキー)　モード変更:B2(Xキー)　ダッシュ:B1(Zキー)");
		if (this->CheckHead())
			ge->debugFont->Draw(ML::Box2D(1000, 20, 500, 500), "頭判定！");
		if (this->CheckFoot())
			ge->debugFont->Draw(ML::Box2D(1060, 20, 500, 500), "足判定！");
		ge->debugFont->Draw(ML::Box2D(1000, 60, 500, 500), "移動ベクトル:"+to_string(this->GetMoveVec().x) + to_string(this->GetMoveVec().y));

		string stateName;
		switch (pState)
		{
		case StateComponent::State::Non:
			stateName = "なし";
			break;
		case StateComponent::State::Idle:
			stateName = "待機";
			break;
		case StateComponent::State::Walk:
			stateName = "歩き";
			break;
		case StateComponent::State::Attack:
			stateName = "攻撃";
			break;
		case StateComponent::State::SpinAttack:
			stateName = "回転攻撃";
			break;
		case StateComponent::State::Damage:
			stateName = "ダメージ";
			break;
		case StateComponent::State::KnockBack:
			stateName = "ノックバック";
			break;
		case StateComponent::State::Dead:
			stateName = "ﾀﾋ亡";
			break;
		case StateComponent::State::Jump:
			stateName = "ジャンプ";
			break;
		case StateComponent::State::Fall:
			stateName = "落下中";
			break;
		case StateComponent::State::Dash:
			stateName = "ダッシュ";
			break;
		case StateComponent::State::Drill:
			stateName = "ドリルモード";
			break;
		case StateComponent::State::DrillDash:
			stateName = "採掘ダッシュ";
			break;
		case StateComponent::State::Mining:
			stateName = "採掘中";
			break;
		case StateComponent::State::Appeal:
			stateName = "アピール";
			break;
		}
		ge->debugFont->Draw(ML::Box2D(1000, 80, 500, 500), "現在のステータス:"+stateName);
		if (StateComponent::State::Drill == this->state_->GetNowState() ||
			StateComponent::State::Mining == this->state_->GetNowState())
		{
			ge->debugFont->Draw(ML::Box2D(1000, 120, 500, 500), "採掘モード");
		}
		else
		{
			ge->debugFont->Draw(ML::Box2D(1000, 120, 500, 500), "行動モード");
		}
		ge->debugFont->Draw(ML::Box2D(1000, 140, 500, 500), "moveCnt:"+to_string(this->state_->moveCnt_));
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
