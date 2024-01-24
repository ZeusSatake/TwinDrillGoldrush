//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_MiningResult.h"
#include  "../Components/Money/PriceTagComponent.h"
#include  "../Components/Money/WalletComponent.h"
#include  "../Components/SecondsTimerComponent.h"
#include  "../System/Task_Save.h"
#include  "../../Scene.h"

namespace MiningResult
{
	Resource::WP  Resource::instance;

	const Object::OreKind Object::sellableOres_[] =
	{
		Object::OreKind::Coal,
		Object::OreKind::Iron,
		Object::OreKind::Gold,
		Object::OreKind::Palladium,
		Object::OreKind::Damascus,
		Object::OreKind::Orichalcum,
		Object::OreKind::HihiIroKane,
		Object::OreKind::Adamantite,
		Object::OreKind::Ladyanite
	};
	string Object::SellableOreName(const OreKind kind)
	{
		switch (kind)
		{
		case Object::OreKind::Coal:
			return "石炭　　　　　";
		case Object::OreKind::Iron:
			return "鉄鉱石　　　　";
		case Object::OreKind::Gold:
			return "金鉱石　　　　";
		case Object::OreKind::Palladium:
			return "パラジウム　";
		case Object::OreKind::Damascus:
			return "ダマスカス　　";
		case Object::OreKind::Orichalcum:
			return "オリハルコン　";
		case Object::OreKind::HihiIroKane:
			return "ヒヒイロカネ　　";
		case Object::OreKind::Adamantite:
			return "アダマンタイト";
		case Object::OreKind::Ladyanite:
			return "レディアナイト";
		default:
			assert(!"設定されていない値です");
		}
	}

	const Object::JewelryKind Object::sellableJewelrys_[] =
	{
		Object::JewelryKind::Diamond,
		Object::JewelryKind::BlackDiamond,
		Object::JewelryKind::PinkDiamond,
		Object::JewelryKind::Emerald,
		Object::JewelryKind::Sapphire,
		Object::JewelryKind::Garnet,
		Object::JewelryKind::Topaz,
		Object::JewelryKind::Amethyst
	};
	string Object::SellableJewelryName(const JewelryKind kind)
	{
		switch (kind)
		{
		case Object::JewelryKind::Diamond:
			return "ダイアモンド　　　　";
		case Object::JewelryKind::BlackDiamond:
			return "ブラックダイアモンド";
		case Object::JewelryKind::PinkDiamond:
			return "ピンクダイアモンド　";
		case Object::JewelryKind::Emerald:
			return "エメラルド　　　　　";
		case Object::JewelryKind::Sapphire:
			return "サファイア　　　　　";
		case Object::JewelryKind::Garnet:
			return "ガーネット　　　　　";
		case Object::JewelryKind::Topaz:
			return "トパーズ　　　　　　";
		case Object::JewelryKind::Amethyst:
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

		render2D_Priority[1] = 0.0f;
		clear_ = false;

		{
			//==================鉱石==================
			//初期化
			for (const auto& ore : sellableOres_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				orePriceTags_.insert(make_pair(ore, priceTag));
				orePriceTags_.at(ore)->Set(SellableOreName(ore), 0);//仮で0
			}
			//値段設定 (値は仮
			orePriceTags_.at(OreKind::Coal)->SetPrice(15);
			orePriceTags_.at(OreKind::Iron)->SetPrice(30);
			orePriceTags_.at(OreKind::Gold)->SetPrice(50);
			orePriceTags_.at(OreKind::Palladium)->SetPrice(70);
			orePriceTags_.at(OreKind::Damascus)->SetPrice(100);
			orePriceTags_.at(OreKind::Orichalcum)->SetPrice(120);
			orePriceTags_.at(OreKind::HihiIroKane)->SetPrice(140);
			orePriceTags_.at(OreKind::Adamantite)->SetPrice(160);
			orePriceTags_.at(OreKind::Ladyanite)->SetPrice(200);

			//==================宝石==================
			//初期化
			for (const auto& jewelry : sellableJewelrys_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				jewelryPriceTags_.insert(make_pair(jewelry, priceTag));
				jewelryPriceTags_.at(jewelry)->Set(SellableJewelryName(jewelry), 0);//仮で0
			}
			//値段設定(値は仮
			jewelryPriceTags_.at(JewelryKind::Diamond)->SetPrice(35);
			jewelryPriceTags_.at(JewelryKind::BlackDiamond)->SetPrice(55);
			jewelryPriceTags_.at(JewelryKind::PinkDiamond)->SetPrice(75);
			jewelryPriceTags_.at(JewelryKind::Emerald)->SetPrice(105);
			jewelryPriceTags_.at(JewelryKind::Sapphire)->SetPrice(125);
			jewelryPriceTags_.at(JewelryKind::Garnet)->SetPrice(145);
			jewelryPriceTags_.at(JewelryKind::Topaz)->SetPrice(165);
			jewelryPriceTags_.at(JewelryKind::Amethyst)->SetPrice(185);
		}

		for (int i = 0; i < size(sellableOres_); ++i)
		{
			getOreCount_.insert(make_pair(sellableOres_[i], 0));
		}
		for (int i = 0; i < size(sellableJewelrys_); ++i)
		{
			getJewelryCount_.insert(make_pair(sellableJewelrys_[i], 0));
		}

		//★タスクの生成
		AddComponent(transitionTimer_ = make_shared<SecondsTimerComponent>(this));
		transitionTimer_->SetCountSeconds(20.0f);

		auto save = Save::Object::Create(true);
		initialHaveMoney_ = save->GetValue<int>(Save::Object::ValueKind::HaveMoney);
		save->Kill();

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
		wallet.Recieve(totalSellingPrice_);
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
		UpdateComponents();

		clear_ = getOreCount_.at(targetOreKind_) == needTargetDestroyAmount_;

		if (clear_ /*&&
			transitionTimer_->IsActive() == false*/)
		{
			//transitionTimer_->Start();
			CalcTotalSellingPrice();

			const auto& inp = ge->in1->GetState();
			if (inp.ST.down)
				Kill();
		}

		if (transitionTimer_->IsCountEndFrame())
			nowScene_->Kill();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//デバッグ表示
		{
			string param = "";
			int restTarget = needTargetDestroyAmount_ - getOreCount_.at(targetOreKind_);
			param += "目標鉱石：" + SellableOreName(targetOreKind_) + " " + to_string(restTarget) + "/" + to_string(needTargetDestroyAmount_) + "個\n";
			//鉱石
			//param += "【鉱石】\n";
			//for (const auto& oreCount : getOreCount_)
			//{
			//	param += SellableOreName(oreCount.first) + "：" + to_string(oreCount.second) + "\n";
			//}

			//宝石
			//param += "【宝石】\n";
			//for (const auto& jewelryCount : getJewelryCount_)
			//{
			//	param += SellableJewelryName(jewelryCount.first) + "：" + to_string(jewelryCount.second) + "\n";
			//}

			ge->debugFont->Draw
			(
				ML::Box2D(ge->screenCenterPos.x - 90, 60, 500, 500),//ML::Box2D(50, 400, 2000, 2000)
				param,
				ML::Color(1.0f, 1.0f, 0.0f, 0.0f)
			);
		}

		if (clear_)
		{//リザルト表示
			string param = "     ====リザルト====\n";

			param += "\n";

			//鉱石
			param += "        【鉱石】\n";
			for (const auto& oreCount : getOreCount_)
			{
				const auto& kind = oreCount.first;
				param += SellableOreName(kind) + "：" + to_string(oreCount.second) + "個 × " + to_string(orePriceTags_.at(kind)->GetPrice()) + "G" + "\n";
			}

			param += "\n";

			//宝石
			param += "           【宝石】\n";
			for (const auto& jewelryCount : getJewelryCount_)
			{
				const auto& kind = jewelryCount.first;
				param += SellableJewelryName(kind) + "：" + to_string(jewelryCount.second) + "個 × " + to_string(jewelryPriceTags_.at(kind)->GetPrice()) + "G" + "\n";
			}

			param += "\n";

			//合計金額
			param += "         【合計金額】\n";
			param += "                 " + to_string(totalSellingPrice_) + "G" + "\n";

			//所持金
			param += "         【所持金】\n";
			param += to_string(initialHaveMoney_ + totalSellingPrice_) + "G" + "\n";

			param += "\n";

			param += "       Press A Key\n";

			ge->debugFont->Draw
			(
				ML::Box2D(550, 160, 2000, 2000),//ML::Box2D(50, 400, 2000, 2000)
				param,
				ML::Color(1.0f, 0.0f, 7.0f, 3.0f)
			);
		}
	}
	void Object::CalcTotalSellingPrice()
	{
		totalSellingPrice_ = 0;
		for (const auto& ore : getOreCount_)
		{
			totalSellingPrice_ += orePriceTags_.at(ore.first)->CalcTotalPrice(ore.second);
		}
		for (const auto& jewelry : getJewelryCount_)
		{
			totalSellingPrice_ += jewelryPriceTags_.at(jewelry.first)->CalcTotalPrice(jewelry.second);
		}
	}
	bool Object::IsSellableOre(const OreKind oreKind)
	{
		for (const auto& sellableOre : sellableOres_)
		{
			if (oreKind == sellableOre)
				return true;
		}
		return false;
	}
	bool Object::IsSellableJewelry(const JewelryKind jewelryKind)
	{
		for (const auto& sellableJewelry : sellableJewelrys_)
		{
			if (jewelryKind == sellableJewelry)
				return true;
		}
		return false;
	}

	void Object::CountUpOre(const OreKind oreKind)
	{
		if (IsSellableOre(oreKind))
			++getOreCount_.at(oreKind);
	}
	void Object::CountUpJewelry(const JewelryKind jewelryKind)
	{
		if (IsSellableJewelry(jewelryKind))
			++getJewelryCount_.at(jewelryKind);
	}

	void Object::SetTargetOre(const OreKind oreKind, const int needDestroyAmount)
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