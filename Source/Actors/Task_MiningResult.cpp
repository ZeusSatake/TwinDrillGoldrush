//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_MiningResult.h"
#include  "../Components/Money/PriceTagComponent.h"
#include  "../Components/Money/WalletComponent.h"
#include  "../System/Task_Save.h"
#include  "../../Scene.h"

namespace MiningResult
{
	Resource::WP  Resource::instance;

	const Map::Object::ChipKind Object::sellableOres_[] =
	{
		Map::Object::ChipKind::Coal,
		Map::Object::ChipKind::Iron,
		Map::Object::ChipKind::Gold,
		Map::Object::ChipKind::HihiIroKane,
		Map::Object::ChipKind::Damascus,
		Map::Object::ChipKind::Orichalcum,
		Map::Object::ChipKind::Palladium,
		Map::Object::ChipKind::Adamantite
	};
	string Object::SellableOreName(const Map::Object::ChipKind kind)
	{
		switch (kind)
		{
			case Map::Object::ChipKind::Coal:
				return "石炭　　　　　";
			case Map::Object::ChipKind::Iron:
				return "鉄鉱石　　　　";
			case Map::Object::ChipKind::Gold:
				return "金鉱石　　　　";
			case Map::Object::ChipKind::HihiIroKane:
				return "ヒヒイロカネ　";
			case Map::Object::ChipKind::Damascus:
				return "ダマスカス　　";
			case Map::Object::ChipKind::Orichalcum:
				return "オリハルコン　";
			case Map::Object::ChipKind::Palladium:
				return "パラジウム　　";
			case Map::Object::ChipKind::Adamantite:
				return "アダマンタイト";
			default:
				assert(!"設定されていない値です");
		}
	}

	const JewelryMap::Object::ChipKind Object::sellableJewelrys_[] =
	{
		JewelryMap::Object::ChipKind::Diamond,
		JewelryMap::Object::ChipKind::BlackDiamond,
		JewelryMap::Object::ChipKind::PinkDiamond,
		JewelryMap::Object::ChipKind::Emerald,
		JewelryMap::Object::ChipKind::Sapphire,
		JewelryMap::Object::ChipKind::Garnet,
		JewelryMap::Object::ChipKind::Topaz,
		JewelryMap::Object::ChipKind::Amethyst
	};
	string Object::SellableJewelryName(const JewelryMap::Object::ChipKind kind)
	{
		switch (kind)
		{
			case JewelryMap::Object::ChipKind::Diamond:
				return "ダイアモンド　　　　";
			case JewelryMap::Object::ChipKind::BlackDiamond:
				return "ブラックダイアモンド";
			case JewelryMap::Object::ChipKind::PinkDiamond:
				return "ピンクダイアモンド　";
			case JewelryMap::Object::ChipKind::Emerald:
				return "エメラルド　　　　　";
			case JewelryMap::Object::ChipKind::Sapphire:
				return "サファイア　　　　　";
			case JewelryMap::Object::ChipKind::Garnet:
				return "ガーネット　　　　　";
			case JewelryMap::Object::ChipKind::Topaz:
				return "トパーズ　　　　　　";
			case JewelryMap::Object::ChipKind::Amethyst:
				return "アメジスト　　　　　";
			default:
				assert(!"設定されていない値です");
		}
	}
	
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		{//鉱石の値段設定
			for (const auto& ore : sellableOres_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				orePriceTags_.insert(make_pair(ore, priceTag));
				orePriceTags_.at(ore)->Set(SellableOreName(ore), 100);
			}
			for (const auto& jewelry : sellableJewelrys_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				jewelryPriceTags_.insert(make_pair(jewelry, priceTag));
				jewelryPriceTags_.at(jewelry)->Set(SellableJewelryName(jewelry), 100);
			}
		}

		//★データ初期化
		render2D_Priority[1] = 0.0f;
		for (int i = 0; i < size(sellableOres_); ++i)
		{
			getOreCount_.insert(make_pair(sellableOres_[i], 0));
		}
		for (int i = 0; i < size(sellableJewelrys_); ++i)
		{
			getJewelryCount_.insert(make_pair(sellableJewelrys_[i], 0));
		}
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		//仮でタスク終了時
		auto save = Save::Object::Create(true);
		WalletComponent wallet = WalletComponent(this);
		wallet.RoadHaveMoney(save);
		wallet.Recieve(CalcTotalSellingPrice());
		save->SetValue(Save::Object::ValueKind::HaveMoney, wallet.GetBalance());

		save->Kill();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//デバッグ表示
		{
			string param = "";
			//鉱石
			param += "【鉱石】\n";
			for (const auto& oreCount : getOreCount_)
			{
				param += SellableOreName(oreCount.first) + "：" + to_string(oreCount.second) + "\n";
			}

			//宝石
			param += "【宝石】\n";
			for (const auto& jewelryCount : getJewelryCount_)
			{
				param += SellableJewelryName(jewelryCount.first) + "：" + to_string(jewelryCount.second) + "\n";
			}

			ge->debugFont->Draw(ML::Box2D(50, 400, 2000, 2000), param, ML::Color(1.0f, 1.0f, 0.0f, 0.0f));
		}
	}
	int Object::CalcTotalSellingPrice() const
	{
		int totalPrice = 0;

		for (const auto& ore : getOreCount_)
		{
			totalPrice += orePriceTags_.at(ore.first)->CalcTotalPrice(ore.second);
		}
		for (const auto& jewelry : getJewelryCount_)
		{
			totalPrice += jewelryPriceTags_.at(jewelry.first)->CalcTotalPrice(jewelry.second);
		}

		return totalPrice;
	}
	bool Object::IsSellableOre(const Map::Object::ChipKind oreKind)
	{
		for (const auto& sellableOre : sellableOres_)
		{
			if (oreKind == sellableOre)
				return true;
		}
		return false;
	}
	bool Object::IsSellableJewelry(const JewelryMap::Object::ChipKind jewelryKind)
	{
		for (const auto& sellableJewelry : sellableJewelrys_)
		{
			if (jewelryKind == sellableJewelry)
				return true;
		}
		return false;
	}

	void Object::CountUpOre(const Map::Object::ChipKind oreKind)
	{
		if (IsSellableOre(oreKind))
			++getOreCount_.at(oreKind);

		if (oreKind != targetOreKind_)
			return;
		if (getOreCount_.at(oreKind) == needTargetDestroyAmount_)
			nowScene_->Kill();
	}
	void Object::CountUpJewelry(const JewelryMap::Object::ChipKind jewelryKind)
	{
		if (IsSellableJewelry(jewelryKind))
			++getJewelryCount_.at(jewelryKind);
	}

	void Object::SetTargetOre(const Map::Object::ChipKind oreKind, const int needDestroyAmount)
	{
		targetOreKind_ = oreKind;
		needTargetDestroyAmount_ = needDestroyAmount;
	}
	void Object::SetNowSecene(Scene* scene)
	{
		nowScene_ = scene;
		nowScene_->SetNextScene(Scene::Base);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}