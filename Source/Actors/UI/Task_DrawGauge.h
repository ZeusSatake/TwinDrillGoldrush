#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲージ
//-------------------------------------------------------------------
#include "../../../GameEngine_Ver3_83.h"
#include "../../../GameObject.h"

namespace DrawGauge
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("HPBar");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	private:
		ML::Percentage gaugeValue_;

		bool isMaxCharge;
		bool isSupportScroll_;
		bool isVisible_;

		ML::Vec2 pos_;
		ML::Point size_;
	public:

		void Set(const int max, const string& path);
		void Set(const ML::Percentage& value);

		void SetMax(const int max);
		void SetMin(const int min);
		int Getmax() const;

		void SetImg(const string& path);
		void SetDrawSize(const int width, const int height);
		void SetPos(const ML::Vec2& pos);
		void SetPos(const float x, const float y);

		void SetSupportScroll(const bool isSupportScroll);
		void SetVisible(const bool visible);

		bool IsMax() const;
		ML::Point GetSize() const;
	};
}