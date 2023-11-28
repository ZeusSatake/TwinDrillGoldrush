//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_DrawGauge.h"

namespace DrawGauge
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/bar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		img.reset();
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
		this->render2D_Priority[1] = 0.05f;
		pos = ML::Vec2(0, 0);

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
		if (res->img == nullptr)
			assert(!"ゲージのimgがnullptrです");
		
		//背景
		ML::Box2D backDraw(-48, -16, 96, 32);
		ML::Box2D backSrc(0, 0, 96, 32);

		res->img->Draw(backDraw.OffsetCopy(pos), backSrc);

		//中身
		ML::Box2D insideDraw(backDraw);
		insideDraw.w *= gaugeValue_.GetNormalizeValue();
		
		ML::Box2D insideSrc = ML::Box2D(0, 32, 96, 32);
		insideSrc.w *= gaugeValue_.GetNormalizeValue();
		//最大溜めは青
		if (isMaxCharge)
			insideSrc.y = 64;

		res->img->Draw(insideDraw.OffsetCopy(pos), insideSrc);
	}

	//===================================================================
	//セッター
	//===================================================================
	void Object::Set(const int max, const string& path)
	{
		SetMax(max);
		SetImg(path);
	}
	void Object::Set(const ML::Percentage& value)
	{
		if (value.GetMaxValue() == value.GetMinValue())
		{
			assert(!"maxとminが同じ値だとゲージを描画できません。");
		}

		gaugeValue_ = value;
	}
	void  Object::SetMax(const int max)
	{
		gaugeValue_.SetMaxValue(max);
	}
	void Object::SetMin(const int min)
	{
		gaugeValue_.SetMinValue(min);
	}
	//===================================================================
	//ゲッター
	//===================================================================
	int  Object::Getmax() const
	{
		return gaugeValue_.GetMaxValue();
	}
	bool Object::IsMax() const
	{
		return gaugeValue_.GetPercent() == 100.0f;
	}

	//===================================================================
	//画像
	//===================================================================
	void Object::SetImg(const string& path)
	{
		if (res->img != nullptr) {
			res->img->ReLoad(path);
		}
		else {
			res->img = DG::Image::Create(path);
		}
	}
	//===================================================================

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