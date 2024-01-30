#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../UIBase/PushButton.h"

class PriceTagComponent;
class WalletComponent;
class Player;

namespace BuyButton
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("Button");		//タスク名
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
	class  Object : public  PushButton
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

		DG::Image::SP image_;
		ML::Box2D	  imageSize_;

		shared_ptr<PriceTagComponent> priceTag_;
		weak_ptr<WalletComponent> buyerWallet_;
		std::function<void()> buyEffect_;

		int buyAmount_;

		void OnEvent() override;
	public:
		void SetImage(const string& path);
		void SetDrawSize(const ML::Point& size);
		void SetImageSize(const ML::Point& size);

		void SetPriceTag(const string& name, const int price);
		void SetBuyerWallet(const shared_ptr<WalletComponent>& wallet);
		void SetBuyEffect(const std::function<void()>& buyEffect);
		void SetProduct(const string& name, const int price, const std::function<void()>& buyEffect);
		void SetBuyAmount(const int amount);
		const shared_ptr<const WalletComponent> GetBuyerWallet() const;
		const shared_ptr<const PriceTagComponent> GetPriceTag() const;
	};
}