#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../ActorClasses/SobaPrison.h"

namespace SobaPrison0
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("オブジェクト");	//グループ名
	const  string  defName("蕎麦の獄");		//タスク名
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
		DG::Image::SP  imgNotice;
		DG::Image::SP  imgPrison;
	};
	//-------------------------------------------------------------------
	class  Object : public  SobaPrison
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
	};
}