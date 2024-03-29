#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ブロックマネージャー
//-------------------------------------------------------------------
#include	"../../../BBlocks.h"

namespace	BlockManager
{
	class BlockState
	{
	public:
		int HP;
		int MaxHP;
		int event;
	};
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(	"Blocks");	//グループ名
	const  string  defName(			"BManager");		//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public	BBlocks
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		typedef  shared_ptr<vector<Object::SP>> SPV;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
			//追加したい変数・メソッドはここに追加する
			//BCharaに含まれないモノのみここに追加する
		bool DestroyBlock(const ML::Point pos, int power);
		void InitArray();					//ブロックHPの初期化に使用

		/*vector<ML::Vec2> DamegeBlockPos;
		int interval = 10;*/



	private:
		BlockState  arr[160][160];//yx

		void eventSearch(int y_, int x_);	//ブロックのイベント(特性)を呼び出す

											//名			num		破壊
		void collapseBlock(int x, int y);	//連鎖崩壊	.10		.可
		//void DamegeBlock(int x, int y);		//臭石		.8		.可

		void EffectOnly(ML::Vec2 pos, int n);	//エフェクトのみのブロックに使用
		void SoundOnly(ML::Vec2 pos);			//破壊音のみ(破壊不可ブロックなどに使用
	};
}