#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//採掘場リザルト
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

#include "../Scene/Task_Map.h"
#include "../Scene/Task_JewelryMap.h"

class Scene;
class PriceTagComponent;
class SecondsTimerComponent;
namespace EventEngine { class Object; }

namespace MiningResult
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("MiningResult");	//タスク名
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

	private:
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

		using OreKind = Map::Object::ChipKind;
		using JewelryKind = JewelryMap::Object::ChipKind;


		Scene* nowScene_;
		int nowStage_;

		bool clear_;
		shared_ptr<SecondsTimerComponent> transitionTimer_;
		weak_ptr<EventEngine::Object> clearEvent_;

		int initialHaveMoney_;

		OreKind targetOreKind_;
		int needTargetDestroyAmount_;

		map<OreKind, int> getOreCount_;
		map<JewelryKind, int> getJewelryCount_;

		map<OreKind, shared_ptr<PriceTagComponent>> orePriceTags_;
		map<JewelryKind, shared_ptr<PriceTagComponent>> jewelryPriceTags_;

		int totalSellingPrice_;
		void CalcTotalSellingPrice();

		static const OreKind sellableOres_[];
		static string SellableOreName(const OreKind kind);
		static const JewelryKind sellableJewelrys_[];
		static string SellableJewelryName(const JewelryKind kind);

		static bool IsSellableOre(const OreKind oreKind);
		static bool IsSellableJewelry(const JewelryKind oreKind);
	public:
		void SetTargetOre(const OreKind oreKind, const int needDestroyAmount);
		void SetNowSecene(Scene* scene);
		void CountUpOre(const OreKind oreKind);
		void CountUpJewelry(const JewelryKind jewelryKind);
	};
}