//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_BuyButton.h"
#include  "../../Components/Money/PriceTagComponent.h"
#include  "../Task_Player.h"
#include  "../../Components/Money/WalletComponent.h"

//�f�o�b�O�p
#include  "../../Components/SecondsTimerComponent.h"

namespace BuyButton
{
	Resource::WP  Resource::instance;
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

		//���f�[�^������
		render2D_Priority[1] = 0.8f;
		box_->setHitBase(ML::Box2D(-100, -50, 200, 100));
		
		buyAmount_ = 1;
		SetEnterButton(XI::VGP::B1);
		SetRecieveInputEnable(true);
		SetSelected(false);
		SetMouse(ge->mouse);
		SetResetTime(5.0f);
		
		AddComponent(priceTag_ = make_shared<PriceTagComponent>(this));
		priceTag_->Set("���ݒ�", 0);

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		priceTag_.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (buyerWallet_.lock()->GetBalance() < priceTag_->CalcTotalPrice(buyAmount_))
		{
			SetEnable(false);
		}
		PushButton::UpDate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		Drawtext(ge->debugFont, true);

		ge->debugFont->Draw(
			ML::Box2D(GetPos().x, GetPos().y, 500, 500),
			priceTag_->GetName() + "�F" + to_string(priceTag_->GetPrice())
		);

		if (image_ != nullptr)
		{
			const ML::Box2D& draw(box_->getHitBase());
			image_->Draw(draw.OffsetCopy(GetPos()), imageSize_);
		}
	}

	void Object::OnEvent()
	{
		if (buyerWallet_.lock()->Payment(priceTag_->CalcTotalPrice(buyAmount_)))
		{
			for (int i = 0; i < buyAmount_; ++i)
				buyEffect_();
		}
	}

	void Object::SetImage(const string& path)
	{
		if (image_ == nullptr)
			image_ = DG::Image::Create(path);
		else
			image_->ReLoad(path);
	}

	void Object::SetDrawSize(const ML::Point& size)
	{
		box_->setHitBase(ML::Box2D(size.x * -0.5f, size.y * -0.5f, size.x, size.y));
	}

	void Object::SetImageSize(const ML::Point& size)
	{
		imageSize_ = ML::Box2D(0, 0, size.x, size.y);
	}

	void Object::SetPriceTag(const string& name, const int price)
	{
		priceTag_->Set(name, price);
	}
	void Object::SetBuyerWallet(const shared_ptr<WalletComponent>& wallet)
	{
		buyerWallet_ = wallet;
	}
	void Object::SetBuyEffect(const std::function<void()>& buyEffect)
	{
		buyEffect_ = buyEffect;
	}
	void Object::SetProduct(const string & name, const int price, const std::function<void()>& buyEffect)
	{
		SetPriceTag(name, price);
		SetBuyEffect(buyEffect);
	}
	void Object::SetBuyAmount(const int amount)
	{
		if (amount <= 0)
		{
			assert(!"�w������0�ȉ��ɂ��Ȃ��ł��������B");
		}
		buyAmount_ = amount;
	}
	const shared_ptr<const WalletComponent> Object::GetBuyerWallet() const
	{
		return buyerWallet_.lock();
	}
	const shared_ptr<PriceTagComponent> Object::GetPriceTag()
	{
		return priceTag_;
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