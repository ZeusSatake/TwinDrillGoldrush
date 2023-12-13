//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "ShopScene.h"
#include  "../Actors/UI/SceneChangeButton.h"

#include  "../Components/Money/WalletComponent.h"
#include  "../Components/Money/PriceTagComponent.h"

#include  "../Actors/UI/Task_BuyButton.h"
#include  "../System/Task_Save.h"

namespace ShopScene
{
	Resource::WP  Resource::instance;
	int Object::drill_MaxLevel = 5;
	int Object::defence_MaxLevel = 5;
	int Object::speed_MaxLevel = 5;
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
		ge->debugRectLoad();

		//���^�X�N�̐���

		auto gotoBaseButton = SceneChangeButton::Object::Create(true);
		gotoBaseButton->SetEnterButton(XI::VGP::ST);
		gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
		gotoBaseButton->SetScene(this, Scene::Kind::Base);
		gotoBaseButton->SetText("���_��");
		AddSceneChangeButton(gotoBaseButton);

		auto save = Save::Object::Create(true);

		AddComponent(wallet = make_shared<WalletComponent>(this));
		wallet->Recieve(save->GetValue<int>(Save::Object::ValueKind::HaveMoney));

		{//�w���{�^��
			
			auto button = BuyButton::Object::Create(true);
			button->SetPos(ge->screenCenterPos);
			button->SetMouse(ge->mouse);
			button->SetEnterButton(XI::Mouse::MB::LB);
			button->SetResetTime(1.0f);
			button->SetText("�h��������");
			button->SetProduct
			(
				"�h��������",
				125,
				[save, button](void)
				{
					save->SetValue(Save::Object::ValueKind::HaveMoney, button->GetBuyerWallet()->GetBalance());
					int nowLevel = save->GetValue<int>(Save::Object::ValueKind::DrillLevel);
					if (nowLevel == drill_MaxLevel)
					{
						button->SetRecieveInputEnable(false);
						return;
					}

					int nextLevel = nowLevel + 1;

					save->SetValue(Save::Object::ValueKind::DrillLevel,	nextLevel);

					//�}�b�N�X�ɂȂ����Ƃ��ɓ��͎�t���I��
					if (nextLevel == drill_MaxLevel)
					{
						button->SetRecieveInputEnable(false);
						return;
					}
				}
			);
			button->SetBuyAmount(1);
			button->SetBuyerWallet(wallet);
			
			buttons_.push_back(button);
		}

		AddComponent(priceTag_Iron = make_shared<PriceTagComponent>(this));
		priceTag_Iron->Set("�S�z��", 15);

		AddComponent(priceTag_Bronze = make_shared<PriceTagComponent>(this));
		priceTag_Bronze->Set("���z��", 8);

		AddComponent(priceTag_Gold = make_shared<PriceTagComponent>(this));
		priceTag_Gold->Set("���z��", 100);

		//player_->wallet_->Recieve(150);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->debugRectReset();
		ge->KillAll_G(SceneChangeButton::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			CreateNextScene();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		Scene::UpDate();

		for (const auto& button : buttons_)
		{
			ge->debugRect(button->GetBox()->getHitBase().OffsetCopy(button->GetPos()));
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ge->debugRectDraw();
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "�V���b�v");
		//ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x, ge->screenCenterPos.y, 500, 500), "�c��" + to_string(player_->wallet_->GetBalance()));

		{
			string param =
				priceTag_Iron->GetName() + "�F" + to_string(priceTag_Iron->CalcTotalPrice(1)) + "\n" +
				priceTag_Bronze->GetName() + "�F" + to_string(priceTag_Bronze->CalcTotalPrice(1)) + "\n" +
				priceTag_Gold->GetName() + "�F" + to_string(priceTag_Gold->CalcTotalPrice(1)) + "\n" +
				"�v���C���̏������F" + to_string(wallet->GetBalance());
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 300, ge->screenCenterPos.y, 500, 500), param);
		}
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