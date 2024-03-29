#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//Ì@êUg
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
	//^XNÉèÄéO[v¼ÆÅL¼
	const  string  defGroupName("UI");	//O[v¼
	const  string  defName("MiningResult");	//^XN¼
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
		//ÏXsÂ
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//¶¬û øÍtrueÅ^XNVXeÖ©®o^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//uú»v^XN¶¬ÉPñ¾¯s¤
		void  UpDate()			override;//uÀsvPt[És¤
		void  Render2D_AF()		override;//u2D`ævPt[És¤
		bool  Finalize();		//uI¹v^XNÁÅÉPñ¾¯s¤

		using OreKind = Map::Object::ChipKind;
		using JewelryKind = JewelryMap::Object::ChipKind;


		Scene* nowScene_;
		int nowStage_;

		bool clear_;
		shared_ptr<SecondsTimerComponent> transitionTimer_;
		weak_ptr<EventEngine::Object> clearEvent_;
		weak_ptr<SecondsTimerComponent> limitTimer_;

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