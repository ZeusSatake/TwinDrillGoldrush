#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//イベント実行エンジン
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"
#include "../../MyPG.h"
#include "../../GameObject.h"

namespace  EventEngine
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(		"イベント");	//グループ名
	const  string  defName(				"実行エンジン");	//タスク名
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
	class  Object : public  GameObject
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		
		Resource::SP	res;
	private:
		Object();
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		static WP instance;
		ifstream evFile;

		//イベント実行エンジンを生成します。
		//既にイベント実行エンジンが存在する場合、生成は失敗します
		static Object::SP Create_Mutex();
		//イベントファイル名を登録＆イベント開始
		bool Set(const string& fPath_);
		//一行分のデータを読み込む
		bool ReadLine(string& lineT_);
		//行データに対する処理を行う
		bool Execute(string& hs_, string& ds_);

		//システム制御系
		//イベントフラグのデータの書き換え
		bool EventFlag(stringstream& ss_);
		//条件分岐
		bool If(stringstream& ss_);
		bool Image(stringstream& ss_);
		bool FileSet(stringstream& ss_);
		//bool AppearObject(stringstream& ss_);
		bool MapLoad(stringstream& ss_);
		//bool ModifyChara(stringstream& ss_);
		bool KillObject(stringstream& ss_);
	};
}