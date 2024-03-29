#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"
#include "../../Scene.h"

namespace Save { class Object; }
namespace BuyButton { class Object; }
class WalletComponent;
class PriceTagComponent;

namespace ShopScene
{
	//^XNÉèÄéO[v¼ÆÅL¼
	const  string  defGroupName("Scene");	//O[v¼
	const  string  defName("Shop");	//^XN¼
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

		DG::Image::SP priceTagImage;
	};
	//-------------------------------------------------------------------
	class  Object : public Scene
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

		shared_ptr<Save::Object> save_;

		//vector<shared_ptr<ToggleButton>> buttons_;
		vector<weak_ptr<BuyButton::Object>> buttons_;

		static int drill_MaxLevel;
		static int defence_MaxLevel;
		static int speed_MaxLevel;

		shared_ptr<WalletComponent> wallet;
	public:

	};
}