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
				return "�ΒY�@�@�@�@�@";
			case Map::Object::ChipKind::Iron:
				return "�S�z�΁@�@�@�@";
			case Map::Object::ChipKind::Gold:
				return "���z�΁@�@�@�@";
			case Map::Object::ChipKind::HihiIroKane:
				return "�q�q�C���J�l�@";
			case Map::Object::ChipKind::Damascus:
				return "�_�}�X�J�X�@�@";
			case Map::Object::ChipKind::Orichalcum:
				return "�I���n���R���@";
			case Map::Object::ChipKind::Palladium:
				return "�p���W�E���@�@";
			case Map::Object::ChipKind::Adamantite:
				return "�A�_�}���^�C�g";
			default:
				assert(!"�ݒ肳��Ă��Ȃ��l�ł�");
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
				return "�_�C�A�����h�@�@�@�@";
			case JewelryMap::Object::ChipKind::BlackDiamond:
				return "�u���b�N�_�C�A�����h";
			case JewelryMap::Object::ChipKind::PinkDiamond:
				return "�s���N�_�C�A�����h�@";
			case JewelryMap::Object::ChipKind::Emerald:
				return "�G�������h�@�@�@�@�@";
			case JewelryMap::Object::ChipKind::Sapphire:
				return "�T�t�@�C�A�@�@�@�@�@";
			case JewelryMap::Object::ChipKind::Garnet:
				return "�K�[�l�b�g�@�@�@�@�@";
			case JewelryMap::Object::ChipKind::Topaz:
				return "�g�p�[�Y�@�@�@�@�@�@";
			case JewelryMap::Object::ChipKind::Amethyst:
				return "�A���W�X�g�@�@�@�@�@";
			default:
				assert(!"�ݒ肳��Ă��Ȃ��l�ł�");
		}
	}
	
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		{//�z�΂̒l�i�ݒ�
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

		//���f�[�^������
		render2D_Priority[1] = 0.0f;
		for (int i = 0; i < size(sellableOres_); ++i)
		{
			getOreCount_.insert(make_pair(sellableOres_[i], 0));
		}
		for (int i = 0; i < size(sellableJewelrys_); ++i)
		{
			getJewelryCount_.insert(make_pair(sellableJewelrys_[i], 0));
		}
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		//���Ń^�X�N�I����
		auto save = Save::Object::Create(true);
		WalletComponent wallet = WalletComponent(this);
		wallet.RoadHaveMoney(save);
		wallet.Recieve(CalcTotalSellingPrice());
		save->SetValue(Save::Object::ValueKind::HaveMoney, wallet.GetBalance());

		save->Kill();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�f�o�b�O�\��
		{
			string param = "";
			//�z��
			param += "�y�z�΁z\n";
			for (const auto& oreCount : getOreCount_)
			{
				param += SellableOreName(oreCount.first) + "�F" + to_string(oreCount.second) + "\n";
			}

			//���
			param += "�y��΁z\n";
			for (const auto& jewelryCount : getJewelryCount_)
			{
				param += SellableJewelryName(jewelryCount.first) + "�F" + to_string(jewelryCount.second) + "\n";
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

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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