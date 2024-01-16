//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "BaseScene.h"
#include  <array>

#include  "../Actors/UI/SceneChangeButton.h"
#include  "../Actors/UI/Task_Cursor.h"

namespace BaseScene
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
		ge->debugRectLoad();
		render2D_Priority[1] = 0.0f;

		//���^�X�N�̐���

		{//�V�[���J�ڃ{�^��, �J�[�\���쐬
			auto cursor = Cursor::Object::Create(true);
			cursor->SetPos(ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y + 80));
			cursor->SetEnterButton(XI::VGP::ST);

			XI::Mouse::MB mouseEnterButton = XI::Mouse::MB::LB;
			XI::VGP		  gamePadEnterButton = cursor->GetEnterButton();

			std::array<SceneChangeButton::Object::SetInfo, 4> buttonInfos =
			{
				SceneChangeButton::Object::SetInfo{
					"�^�C�g����",
					this,
					Scene::Kind::Title,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"�V���b�v��",
					this,
					Scene::Kind::Shop,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"�������",
					this,
					Scene::Kind::MartialFight,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				},
				{	"�̌@���",
					this,
					Scene::Kind::Mining,
					mouseEnterButton,
					gamePadEnterButton,
					cursor.get()
				}
			};

			for (int i = 0; i < buttonInfos.size(); ++i)
			{
				const auto& buttonInfo = buttonInfos.at(i);

				auto button = SceneChangeButton::Object::Create(true);
				button->Set(buttonInfo);
				int buttonMargin = 10;
				float startX = ge->screenCenterPos.x - (buttonInfos.size() * 0.5f) * (button->GetBox()->getHitBase().w + buttonMargin) + button->GetBox()->getHitBase().w * 0.5f;
				button->SetPos(
					ML::Vec2(
					startX + ((button->GetBox()->getHitBase().w + buttonMargin) * i),
					ge->screenCenterPos.y));
				AddSceneChangeButton(button);
				buttons_.push_back(button);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_GN(Cursor::defGroupName, Cursor::defName);
		ge->debugRectReset();

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

		for (auto& button : buttons_)
		{
			ge->debugRect(button->GetBox()->getHitBase().OffsetCopy(button->GetPos()));
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "���_");
		ge->debugRectDraw();
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