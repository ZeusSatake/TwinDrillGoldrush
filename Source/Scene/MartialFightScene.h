#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//武闘会
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"
#include "../../Scene.h"

class SecondsTimerComponent;
class GameOverEventComponent;
class BossLady;
namespace Satake { class Object; }
namespace EventEngine { class Object; }

namespace MartialFightScene
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("Scene");	//グループ名
	const  string  defName("MartialFight");	//タスク名
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
	class  Object : public  Scene
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

		int nowStage_;

		int enemyCount_;
		weak_ptr<BossLady> boss_;
		bool clear_;
		bool spawnableBoss_;

		weak_ptr<EventEngine::Object> bossEvent_;
		weak_ptr<EventEngine::Object> clearEvent_;

		shared_ptr<GameOverEventComponent> gameOverEvent_;
		//string debugMsg;
		//shared_ptr<SecondsTimerComponent> debugTimer;

		bool IsEndOfClearEvent();
	public:
		void SetBoss(const shared_ptr<BossLady>& boss);

		void DecreaseEnemyCount();

		void SpawnBoss();

		bool EndOfSpawnBossEvent();
	};
}