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
			return "�ΒY�@�@�@�@�@";
		case Object::OreKind::Iron:
			return "�S�z�΁@�@�@�@";
		case Object::OreKind::Gold:
			return "���z�΁@�@�@�@";
		case Object::OreKind::Palladium:
			return "�p���W�E���@";
		case Object::OreKind::Damascus:
			return "�_�}�X�J�X�@�@";
		case Object::OreKind::Orichalcum:
			return "�I���n���R���@";
		case Object::OreKind::HihiIroKane:
			return "�q�q�C���J�l�@�@";
		case Object::OreKind::Adamantite:
			return "�A�_�}���^�C�g";
		case Object::OreKind::Ladyanite:
			return "���f�B�A�i�C�g";
		default:
			assert(!"�ݒ肳��Ă��Ȃ��l�ł�");
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
			return "�_�C�A�����h�@�@�@�@";
		case Object::JewelryKind::BlackDiamond:
			return "�u���b�N�_�C�A�����h";
		case Object::JewelryKind::PinkDiamond:
			return "�s���N�_�C�A�����h�@";
		case Object::JewelryKind::Emerald:
			return "�G�������h�@�@�@�@�@";
		case Object::JewelryKind::Sapphire:
			return "�T�t�@�C�A�@�@�@�@�@";
		case Object::JewelryKind::Garnet:
			return "�K�[�l�b�g�@�@�@�@�@";
		case Object::JewelryKind::Topaz:
			return "�g�p�[�Y�@�@�@�@�@�@";
		case Object::JewelryKind::Amethyst:
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

		render2D_Priority[1] = 0.0f;
		clear_ = false;

		{
			//==================�z��==================
			//������
			for (const auto& ore : sellableOres_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				orePriceTags_.insert(make_pair(ore, priceTag));
				orePriceTags_.at(ore)->Set(SellableOreName(ore), 0);//����0
			}
			//�l�i�ݒ� (�l�͉�
			orePriceTags_.at(OreKind::Coal)->SetPrice(15);
			orePriceTags_.at(OreKind::Iron)->SetPrice(30);
			orePriceTags_.at(OreKind::Gold)->SetPrice(50);
			orePriceTags_.at(OreKind::Palladium)->SetPrice(70);
			orePriceTags_.at(OreKind::Damascus)->SetPrice(100);
			orePriceTags_.at(OreKind::Orichalcum)->SetPrice(120);
			orePriceTags_.at(OreKind::HihiIroKane)->SetPrice(140);
			orePriceTags_.at(OreKind::Adamantite)->SetPrice(160);
			orePriceTags_.at(OreKind::Ladyanite)->SetPrice(200);

			//==================���==================
			//������
			for (const auto& jewelry : sellableJewelrys_)
			{
				shared_ptr<PriceTagComponent> priceTag;
				AddComponent(priceTag = make_shared<PriceTagComponent>(this));
				jewelryPriceTags_.insert(make_pair(jewelry, priceTag));
				jewelryPriceTags_.at(jewelry)->Set(SellableJewelryName(jewelry), 0);//����0
			}
			//�l�i�ݒ�(�l�͉�
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

		//���^�X�N�̐���
		AddComponent(transitionTimer_ = make_shared<SecondsTimerComponent>(this));
		transitionTimer_->SetCountSeconds(20.0f);

		auto save = Save::Object::Create(true);
		initialHaveMoney_ = save->GetValue<int>(Save::Object::ValueKind::HaveMoney);
		save->Kill();

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
		wallet.Recieve(totalSellingPrice_);
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
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�f�o�b�O�\��
		{
			string param = "";
			int restTarget = needTargetDestroyAmount_ - getOreCount_.at(targetOreKind_);
			param += "�ڕW�z�΁F" + SellableOreName(targetOreKind_) + " " + to_string(restTarget) + "/" + to_string(needTargetDestroyAmount_) + "��\n";
			//�z��
			//param += "�y�z�΁z\n";
			//for (const auto& oreCount : getOreCount_)
			//{
			//	param += SellableOreName(oreCount.first) + "�F" + to_string(oreCount.second) + "\n";
			//}

			//���
			//param += "�y��΁z\n";
			//for (const auto& jewelryCount : getJewelryCount_)
			//{
			//	param += SellableJewelryName(jewelryCount.first) + "�F" + to_string(jewelryCount.second) + "\n";
			//}

			ge->debugFont->Draw
			(
				ML::Box2D(ge->screenCenterPos.x - 90, 60, 500, 500),//ML::Box2D(50, 400, 2000, 2000)
				param,
				ML::Color(1.0f, 1.0f, 0.0f, 0.0f)
			);
		}

		if (clear_)
		{//���U���g�\��
			string param = "     ====���U���g====\n";

			param += "\n";

			//�z��
			param += "        �y�z�΁z\n";
			for (const auto& oreCount : getOreCount_)
			{
				const auto& kind = oreCount.first;
				param += SellableOreName(kind) + "�F" + to_string(oreCount.second) + "�� �~ " + to_string(orePriceTags_.at(kind)->GetPrice()) + "G" + "\n";
			}

			param += "\n";

			//���
			param += "           �y��΁z\n";
			for (const auto& jewelryCount : getJewelryCount_)
			{
				const auto& kind = jewelryCount.first;
				param += SellableJewelryName(kind) + "�F" + to_string(jewelryCount.second) + "�� �~ " + to_string(jewelryPriceTags_.at(kind)->GetPrice()) + "G" + "\n";
			}

			param += "\n";

			//���v���z
			param += "         �y���v���z�z\n";
			param += "                 " + to_string(totalSellingPrice_) + "G" + "\n";

			//������
			param += "         �y�������z\n";
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