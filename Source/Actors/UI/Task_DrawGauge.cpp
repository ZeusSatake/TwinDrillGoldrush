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
		this->render2D_Priority[1] = 0.01f;
		pos_ = ML::Vec2(0, 0);
		isVisible_ = true;
		isSupportScroll_ = true;

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
		if (isVisible_ == false)
			return;

		if (res->img == nullptr)
			assert(!"ゲージのimgがnullptrです");
		
		//背景
		ML::Box2D backDraw(-(size_.x / 2), -(size_.y / 2), size_.x, size_.y);
		//ML::Box2D backSrc(0, 0, 96, 32);

		//スクロール対応
		if (isSupportScroll_)
			backDraw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		res->img->Draw(backDraw.OffsetCopy(pos_), backSrc_);
		
		//中身
		ML::Box2D insideDraw(backDraw);
		insideDraw.w *= gaugeValue_.GetNormalizeValue();
		
		auto insideSrc = insideSrc_;
		insideSrc.w = insideSrc_.w * gaugeValue_.GetNormalizeValue();

		res->img->Draw(insideDraw.OffsetCopy(pos_), insideSrc);
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
	void Object::SetPos(const ML::Vec2& pos)
	{
		pos_ = pos;
	}
	void Object::SetPos(const float x, const float y)
	{
		SetPos(ML::Vec2(x, y));
	}
	void Object::SetDrawSize(const int width, const int height)
	{
		size_.x = width;
		size_.y = height;
	}
	void Object::SetImg(const string& path)
	{
		if (res->img != nullptr) {
			res->img->ReLoad(path);
		}
		else {
			res->img = DG::Image::Create(path);
		}
	}
	void Object::SetSupportScroll(const bool isSupportScroll)
	{
		isSupportScroll_ = isSupportScroll;
	}
	void Object::SetVisible(const bool visible)
	{
		isVisible_ = visible;
	}
	void Object::SetBackSrc(const ML::Box2D& backSrc)
	{
		backSrc_ = backSrc;
	}
	void Object::SetInsideSrc(const ML::Box2D& insideSrc)
	{
		insideSrc_ = insideSrc;
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
	ML::Point Object::GetSize() const
	{
		return size_;
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