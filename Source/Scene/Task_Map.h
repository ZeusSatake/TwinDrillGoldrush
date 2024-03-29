#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../Actor.h"

namespace BlockManager { class Object; }
namespace MiningResult { class Object; }

namespace  Map
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("本編");	//グループ名
	const  string  defName("マップ");	//タスク名
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
	class  Object : public  Actor
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
		//追加したい変数・メソッドはここに追加する
		DG::Image::SP img;
		int arr[160][160];//yx
		int chipSize;
		ML::Box2D			hitBase;//ピクセル単位のマップサイズを持つ
		ML::Box2D  chip[32];
		shared_ptr<BlockManager::Object> blockManager_;
		weak_ptr<MiningResult::Object> miningResult_;
	public:
		enum class ChipKind
		{
			Empty,
			Soil,
			HardSoil,
			MossySoil,
			LightSoil,
			RoughSoil,
			Stone,
			BedRock,
			MossyStone,
			Coal,
			CollapseStone,
			Iron,
			Gold,
			Palladium,
			Damascus,
			Orichalcum,
			HihiIroKane,
			Adamantite,
			DiagonalBrick = 26,
			Brick,
			Ladyanite
		};

		int					sizeY, sizeX;
		bool  Load(const  string& fileName);//マップ読み込み
		bool  CheckHit(const  ML::Box2D& hit);//あたり判定
		void AdjustCameraPos();//マップ外を見せないようにカメラを位置調整する
		void  Search(const ML::Vec2& pos_);//対象ブロック検索
		inline int GetMapChip(int y, int x) { return this->arr[y][x]; } 
		inline void SetMapChip(int y, int x, int no) { arr[y][x] = no; }
	};
}
		