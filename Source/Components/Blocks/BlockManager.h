#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ブロックマネージャー
//-------------------------------------------------------------------
#include	"../../../BBlocks.h"

namespace	Manager
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
		void Damage(const ML::Point pos, int power);
		void InitArray();					//ブロックHPの初期化に使用

	private:
		BlockState  arr[160][160];//yx

		void eventSearch(int y_, int x_);	//ブロックのイベント(特性)を呼び出す		
		//名			num		破壊
		void Stone(ML::Vec2 pos);			//石			.6		.可
		void Bedrock(ML::Vec2 pos);			//岩盤		.7		.不可
		void collapseBlock(int x, int y);	//連鎖崩壊	.10		.可
		void IronOre(ML::Vec2 pos);			//鉄鉱石		.11		.可
	};
}