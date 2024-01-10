#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h
#include "../../GameEngine_Ver3_83.h"

#include "../Scene/Task_Map.h"
#include "../Scene/Task_JewelryMap.h"
========
#include "../../EnemyClasses/LadySatake.h"
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h

namespace Satake
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵");	//グループ名
	const  string  defName("佐竹お嬢様");//タスク名
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
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h

	private:
========
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		DG::Image::SP img;
		DG::Image::SP fanImg;
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
	};
	//-------------------------------------------------------------------
	class  Object : public  LadySatake
	{
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
========
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
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

		map<Map::Object::ChipKind, shared_ptr<PriceTagComponent>> orePriceTags_;
		map<JewelryMap::Object::ChipKind, shared_ptr<PriceTagComponent>> jewelryPriceTags_;

		map<Map::Object::ChipKind, int> getOreCount_;
		map<JewelryMap::Object::ChipKind, int> getJewelryCount_;

		static const Map::Object::ChipKind sellableOres_[];
		static string SellableOreName(const Map::Object::ChipKind kind);
		static const JewelryMap::Object::ChipKind sellableJewelrys_[];
		static string SellableJewelryName(const JewelryMap::Object::ChipKind kind);

		static bool IsSellableOre(const Map::Object::ChipKind oreKind);
		static bool IsSellableJewelry(const JewelryMap::Object::ChipKind oreKind);

		int CalcTotalPrice() const;
	public:
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h

		void CountUpOre(const Map::Object::ChipKind oreKind);
		void CountUpJewelry(const JewelryMap::Object::ChipKind jewelryKind);
========
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
	};
}