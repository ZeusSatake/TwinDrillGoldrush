//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  <array>
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

		save_ = Save::Object::Create(true);

		AddComponent(wallet = make_shared<WalletComponent>(this));
		wallet->RoadHaveMoney(save_);

		{//�w���{�^��
			array<shared_ptr<BuyButton::Object>, 3> levelButtons;
			struct LevelButtonInfo
			{
				string name;
				int price;
				Save::Object::ValueKind kind;
				int max;
			};

			enum class levelButtonKind
			{
				Drill,
				Dress,
				Speed
			};

			LevelButtonInfo levelButtonInfos[levelButtons.size()] =
			{
				{
					"�h��������",
					125,
					Save::Object::ValueKind::DrillLevel,
					5
				},
				{
					"��",
					520,
					Save::Object::ValueKind::DefenceLevel,
					5
				},
				{
					"���x����",
					330,
					Save::Object::ValueKind::SpeedLevel,
					5
				}
			};
			for (int i = 0; i < levelButtons.size(); ++i)
			{
				auto& button = levelButtons[i] = BuyButton::Object::Create(true);
				auto& info = levelButtonInfos[i];
				//button->SetPosX(300 + button->GetBox()->getHitBase().w * i);
				//button->SetPosY(ge->screenCenterPos.y);
				button->SetMouse(ge->mouse);
				button->SetEnterButton(XI::Mouse::MB::LB);
				button->SetResetTime(1.0f);
				button->SetBuyAmount(1);
				button->SetBuyerWallet(wallet);
				button->SetProduct
				(
					info.name,
					info.price,
					[this, button, info](void)
					{
						save_->SetValue(Save::Object::ValueKind::HaveMoney, button->GetBuyerWallet()->GetBalance());
						int nowLevel = save_->GetValue<int>(info.kind);
						if (nowLevel >= info.max)
						{
							button->SetEnable(false);
							return;
						}

						int nextLevel = nowLevel + 1;

						save_->SetValue(info.kind, nextLevel);

						//�}�b�N�X�ɂȂ����Ƃ��ɓ��͎�t���I��
						if (nextLevel >= drill_MaxLevel)
						{
							button->SetEnable(false);
							return;
						}
					}
				);

				//���x���ő�̂��͖̂�����
				if (save_->GetValue<int>(info.kind) >= info.max)
				{
					button->SetEnable(false);
				}
				buttons_.push_back(button);
			}

			//�ʒu����
			levelButtons.at((int)levelButtonKind::Drill)->SetPos(ML::Vec2(ge->screenCenterPos.x, 150));
			levelButtons.at((int)levelButtonKind::Dress)->SetPos(ML::Vec2(280, ge->screenCenterPos.y + 120));
			levelButtons.at((int)levelButtonKind::Speed)->SetPos(ML::Vec2(ge->screenWidth - 280, ge->screenCenterPos.y + 120));
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->debugRectReset();
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(BuyButton::defGroupName, BuyButton::defName);
		save_->Kill();

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

		{
			string param =
				"�v���C���̏������F" + to_string(wallet->GetBalance()) + "\n" + 
				"�h�����F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DrillLevel)) + "\n" + 
				"�h��@�F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DefenceLevel)) + "\n" + 
				"���x�@�F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::SpeedLevel));
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 350, 200, 500, 500), param);
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