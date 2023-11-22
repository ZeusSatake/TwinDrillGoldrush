//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "GameScene.h"

#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "EndingScene.h"

#include  "../System/Task_BackGround.h"
#include  "Task_Map.h"

#include  "../Actors/UI/SceneChangeButton.h"

namespace  GameScene
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
		this->render2D_Priority[1] = 0.0f;
		ge->debugRectLoad();

		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->gameScreenWidth = ge->screenWidth;
		
		fontImg.img = DG::Image::Create("./data/image/font_number.png");
		fontImg.size = ML::Point{ 20, 32 };
		ge->score = 0;
		
		//�f�o�b�O�p�t�H���g�̏���
		this->TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���^�X�N�̐���


		{//�w�i�^�X�N����
			ML::Point imgSize = { 800, 600 };
			ML::Point drawSize = { (int)ge->screenWidth, (int)ge->screenHeight };
			int sprit = 1;
			auto back = BackGround::Object::Create(true);
			back->SetUp("./data/image/back.png",
						imgSize,
						drawSize,
						BackGround::Object::RenderSize::FullScreen,
						sprit);
		}
		
		{//�� �z��
			auto map = Map::Object::Create(true);
			map->Load("map_stone");
		}
		{//���
			auto mapore = Map::Object::Create(true);
			mapore->Load("map_jewelry");
			mapore->render2D_Priority[1] = 0.85f;
		}

		{//���_�ɖ߂�{�^��(�f�o�b�O�p
			auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("���_��");
			AddSceneChangeButton(gotoBaseButton);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		ge->KillAll_G("�{��");
		ge->KillAll_G("�V�X�e��");
		ge->KillAll_G(SceneChangeButton::defGroupName);

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

		auto inp = ge->in1->GetState();
		if (inp.SE.down) {
			this->Kill();
		}
		if (ge->GameOverFlag) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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