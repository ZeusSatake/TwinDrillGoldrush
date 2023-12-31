#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//背景表示
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

namespace BackGround
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("システム");	//グループ名
	const  string  defName("背景");	//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
	public:
		//描画モード
		enum class RenderSize {
			FullScreen,			//全画面
			Sprit_Horizontal,	//分割 縦
			Sprit_Vertical,		//分割 横
		};
		enum class ScrollDirection {
			Up	 ,
			Down ,
			Left ,
			Right,
		};
	private:
		DG::Image::SP img;
		string imgPath;
		ML::Point imgSize;
		int spritNum;
		RenderSize renderSize;
		ML::Point drawSize_Sprit;
		ML::Point drawSize_Full;

		//スクロール用変数
		bool isScroll;
		ML::Vec2 pos;
		ML::Vec2 resetPos;
		ML::Vec2 scrollDirTable[4] = {
			ML::Vec2( 0, -1),
			ML::Vec2( 0,  1),
			ML::Vec2(-1,  0),
			ML::Vec2( 1,  0),
		};
		ScrollDirection scrollDirection;
		int scrollSpeed;
		void CreateSubScreen();
	protected:
		bool isSubScreen;
	public:	
		//セットアップ パス、サイズ
		void SetUp(string imgPath_, ML::Point imgSize_, ML::Point drawSize_, RenderSize renderSize_ = RenderSize::FullScreen, int spritNum_ = 1);
		//描画サイズ設定
		void SetDrawSize(ML::Point drawSize_, RenderSize renderMode_, int spritNum_);
		void SetScroll(ML::Vec2 pos_, int speed, ScrollDirection scrollDirection_);
		ML::Point GetDrawSize();
	};
}