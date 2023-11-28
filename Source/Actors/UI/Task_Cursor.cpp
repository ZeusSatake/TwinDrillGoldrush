//-------------------------------------------------------------------
//UIカーソル
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_Cursor.h"
#include  "../../Components/Movement.h"
#include  "../../../ToggleButton.h"

namespace Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		image_ = DG::Image::Create("./data/image/shot.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		image_.reset();
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
		this->pos_ = ge->screenCenterPos;

		AddComponent(movement_ = make_shared<Movement>(this));
		//パラメータの設定
		movement_->SetSpeed(8.0f, 15.0f, 0.5f);
		movement_->SetDecelerationRate(ML::Percentage(30.0f));
		movement_->SetAcceleration(20.0f);

		box_->setHitBase(ML::Box2D(-28, -28, 56, 56));
		
		auto buttons = ge->GetTasks<ToggleButton>("UI", "Button");
		for (auto& button : *buttons)
		{
			button->SetSelector(this);
		}
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		movement_.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		movement_->LStickInputToMove(ge->in1);
		
		{//ボタンとのあたり判定 重なっているものを選択状態にする
			//auto mouse = ge->mouse->GetState();

			//auto buttons = ge->GetTasks<ToggleButton>("UI", "Button");
			//const ML::Box2D& myBox = this->box_->getHitBase().OffsetCopy(this->pos_);
			//for (auto& button : *buttons)
			//{
			//	if (button->CheckHit(ML::Box2D(mouse.pos.x, mouse.pos.y, 1, 1)) ||
			//		button->CheckHit(myBox))
			//		button->SetSelected(true);
			//	else
			//		button->SetSelected(false);
			//}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 56, 56);
		res->image_->Draw(box_->getHitBase().OffsetCopy(this->pos_), src);
	}

	void Object::SetEnterButton(const XI::VGP enterButton)
	{
		enterButton_ = enterButton;
	}
	XI::VGP Object::GetEnterButton() const
	{
		return enterButton_;
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