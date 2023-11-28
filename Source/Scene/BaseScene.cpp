//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "BaseScene.h"

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

		//���^�X�N�̐���

		auto cursor = Cursor::Object::Create(true);
		cursor->SetEnterButton(XI::VGP::ST);

		{//�^�C�g���ɑJ�ڂ���{�^��
			auto button = SceneChangeButton::Object::Create(true);
			button->SetText("�^�C�g����");
			button->SetScene(this, Scene::Kind::Title);
			button->SetEnterButton(XI::Mouse::MB::LB);
			button->SetSelector(cursor.get());
			button->SetEnterButton(cursor->GetEnterButton());
			AddSceneChangeButton(button);
		}

		{//�V���b�v�ɑJ�ڂ���{�^��
			auto button = SceneChangeButton::Object::Create(true);
			button->SetText("�V���b�v��");
			button->SetScene(this, Scene::Kind::Shop);
			button->SetEnterButton(XI::Mouse::MB::LB);
			button->SetSelector(cursor.get());
			button->SetEnterButton(cursor->GetEnterButton());
			button->pos_ = ML::Vec2(ge->screenCenterPos.x - 200, ge->screenCenterPos.y);
			AddSceneChangeButton(button);
		}

		{//������ɑJ�ڂ���{�^��
			auto button = SceneChangeButton::Object::Create(true);
			button->SetText("�������");
			button->SetScene(this, Scene::Kind::MartialFight);
			button->SetEnterButton(XI::Mouse::MB::LB);
			button->SetSelector(cursor.get());
			button->SetEnterButton(cursor->GetEnterButton());
			button->pos_ = ML::Vec2(ge->screenCenterPos.x, ge->screenCenterPos.y);
			AddSceneChangeButton(button);
		}

		{//�̌@��ɑJ�ڂ���{�^��
			auto button = SceneChangeButton::Object::Create(true);
			button->SetText("�̌@���");
			button->SetScene(this, Scene::Kind::Mining);
			button->SetEnterButton(XI::Mouse::MB::LB);
			button->SetSelector(cursor.get());
			button->SetEnterButton(cursor->GetEnterButton());
			button->pos_ = ML::Vec2(ge->screenCenterPos.x + 200, ge->screenCenterPos.y);
			AddSceneChangeButton(button);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "���_");
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