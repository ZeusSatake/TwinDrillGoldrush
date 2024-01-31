//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  <array>
#include  "../../MyPG.h"
#include  "ShopScene.h"
#include  "../../sound.h"
#include  "../Components/Money/WalletComponent.h"
#include  "../Components/Money/PriceTagComponent.h"

#include  "../Actors/UI/SceneChangeButton.h"
#include  "../Actors/UI/Task_BuyButton.h"
#include  "../Actors/UI/Task_Cursor.h"
#include  "../Event/Task_EventEngine.h"
#include  "../System/Task_BackGround.h"
#include  "../System/Task_Save.h"

#include  "../Actors/UI/Task_UIBanner.h"

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
		priceTagImage = DG::Image::Create("./data/image/ui/priceTag/bookmark.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		priceTagImage.reset();
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
		render2D_Priority[1] = 0.0f;
		bgm::LoadFile("shop", "./data/sound/shop.mp3");
		//���^�X�N�̐���

		auto cursor = Cursor::Object::Create(true);
		cursor->SetPos(ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y + 80));
		cursor->SetEnterButton(XI::VGP::B1);

		auto gotoBaseButton = SceneChangeButton::Object::Create(true);
		{
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("���_��");
			gotoBaseButton->SetImage("./data/image/ui/���_�J��.png");
			ML::Point size{ 200, 100 };
			gotoBaseButton->SetSize(ML::Point{ 200, 100 });
			gotoBaseButton->SetPos(ML::Vec2(size.x * 0.5f, size.y * 0.5f));
		}
		
		AddSceneChangeButton(gotoBaseButton);

		save_ = Save::Object::Create(true);

		AddComponent(wallet = make_shared<WalletComponent>(this));
		wallet->RoadHaveMoney(save_);

		{//�w���{�^��
			array<shared_ptr<BuyButton::Object>, 3> levelButtons;
			struct LevelButtonInfo
			{
				string name;
				string imagePath;
				array<int, 5> prices;
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
					"�̌@�͋���",
					"./data/image/ui/buyButton/��э�.png",
					{ 250, 500, 1000, 1750, 2500 },
					Save::Object::ValueKind::DrillLevel,
					5
				},
				{
					"�h��͋���",
					"./data/image/ui/buyButton/�h���X.png",
					{ 300, 600, 1200, 2000, 2750 },
					Save::Object::ValueKind::DefenceLevel,
					5
				},
				{
					"���x����",
					"./data/image/ui/buyButton/�C.png",
					{ 250, 500, 1000, 1750, 2500 },
					Save::Object::ValueKind::SpeedLevel,
					5
				}
			};
			for (int i = 0; i < levelButtons.size(); ++i)
			{
				auto& button = levelButtons[i] = BuyButton::Object::Create(true);
				auto& info = levelButtonInfos[i];
				button->SetImage(info.imagePath);
				button->SetDrawSize(ML::Point{384, 384});
				button->SetImageSize(ML::Point{ 384, 384 });

				button->SetMouse(ge->mouse);
				button->SetEnterButton(XI::Mouse::MB::LB);
				button->SetSelector(cursor.get());
				button->SetEnterButton(cursor->GetEnterButton());

				button->SetResetTime(1.0f);
				button->SetBuyAmount(1);
				button->SetBuyerWallet(wallet);
				
				button->SetProduct
				(
					info.name,
					info.prices.at(save_->GetValue<int>(info.kind) - 1),
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
						button->GetPriceTag()->SetPrice(info.prices.at(save_->GetValue<int>(info.kind) - 1));

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
			ML::Point buttonSize{ levelButtons.at(0)->GetBox()->getHitBase().w, levelButtons.at(0)->GetBox()->getHitBase().h };
			ML::Point buttonHalfSize{ buttonSize.x * 0.5f, buttonSize.y * 0.5f };
			int margin = 30;
			for (int i = 0; i < levelButtons.size(); ++i)
			{
				int xMargin = margin + margin * i;
				int xPadding = buttonHalfSize.x + buttonSize.x * i;
				levelButtons.at(i)->SetPos(ML::Vec2(xPadding + xMargin, buttonHalfSize.y + 170));
			}
		}

		if (save_->GetValue<int>(Save::Object::ValueKind::EndOfShopTutorial) == 0)
		{
			auto ev = EventEngine::Object::Create_Mutex();
			ev->Set("./data/event/EventShopTutorial.txt");
			save_->SetValue(Save::Object::ValueKind::EndOfShopTutorial, 1.0f);
		}

		{//�w�i�^�X�N����
			ML::Point imgSize = { 640, 516 };
			ML::Point drawSize = { (int)ge->screenWidth, (int)ge->screenHeight };
			int sprit = 1;
			auto back = BackGround::Object::Create(true);
			back->SetUp("./data/image/backGround/shop/shopBackGround.png",
				imgSize,
				drawSize,
				BackGround::Object::RenderSize::FullScreen,
				sprit);
		}

		{//UI�o�i�[
			auto banner = UIBanner::Object::Create(true);
			banner->SetDrawSize(ML::Box2D(0, 0, 256, 128));
			banner->SetDrawArea(UIBanner::Object::DrawArea::RightTop);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_GN(Cursor::defGroupName, Cursor::defName);
		ge->KillAll_GN(BackGround::defGroupName, BackGround::defName);
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(BuyButton::defGroupName, BuyButton::defName);
		save_->Kill();
		bgm::Stop("shop");
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
		bgm::Play("shop");
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			string param =
				"�������F" + to_string(wallet->GetBalance()) + "G\n" + 
				"�h����Lv�F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DrillLevel)) + "\n" + 
				"�h���XLv�F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::DefenceLevel)) + "\n" + 
				"�CLv  �@�F" + to_string(save_->GetValue<int>(Save::Object::ValueKind::SpeedLevel));
			ge->debugFont->Draw(ML::Box2D(ge->screenWidth - 200, 30, 500, 500), param, ML::Color(1, 0, 0, 0));
		}

		//�l�D�\��
		for (const auto& button : buttons_)
		{
			const auto buttonSP = button.lock();
			ML::Box2D src(0, 0, 401, 170);
			ML::Box2D draw(-80, -35, 160, 70);

			ML::Vec2 pos(buttonSP->GetPos().x - buttonSP->GetBox()->getHitBase().w * 0.3f, buttonSP->GetPos().y - buttonSP->GetBox()->getHitBase().h * 0.075f);
			
			res->priceTagImage->Draw(draw.OffsetCopy(pos), src);
			ge->debugFont->Draw(
				ML::Box2D(pos.x, pos.y - (ge->debugFont->GetHeight() * 1.5f), 300, 300),
				to_string(button.lock()->GetPriceTag()->GetPrice()) + "G",
				ML::Color(1, 0, 0, 0));
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