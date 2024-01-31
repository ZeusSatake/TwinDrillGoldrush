//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "EndingScene.h"
#include  "TitleScene.h"
#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "../System/Task_FlashDraw.h"
#include  "../System/Task_BackGround.h"
#include  "../Event/Task_EventEngine.h"

namespace  EndingScene
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		resultImg = DG::Image::Create("./data/image/result.png");
		rankImg = DG::Image::Create("./data/image/rank.png");
		font = DG::Font::Create("�l�r �S�V�b�N", 32, 64);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		resultImg.reset();
		rankImg.reset();
		font.reset();
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
		frameCount = 0;
		bgm::LoadFile("ending", "./data/sound/ending.mp3");
		//���^�X�N�̐���
		{//�摜�t�H���g(�X�R�A
			//int digit = 4;
			//ML::Box2D draw(0, 0, 48, 64);
			//ML::Box2D src(0, 0, 24, 32);
			//ML::Vec2 pos = ge->screenCenterPos;
			//pos.x -= draw.w * 0.5f * digit;
			//pos.y -= draw.h * 0.5f - 50;
			//drawScore = DrawNumFont::Object::Create(true);
			//drawScore->SetUp("./data/image/notext.png", src, draw, digit, pos);
		}

		{//�w�i�^�X�N����
			ML::Point imgSize;
			imgSize.x = 800;
			imgSize.y = 600;

			ML::Point drawSize;
			drawSize.x = (int)ge->screenWidth;
			drawSize.y = (int)ge->screenHeight;

			auto backGround = BackGround::Object::Create(true);
			backGround->SetUp("./data/image/back.png", imgSize, drawSize);
		}

		{
			endingEvent_ = EventEngine::Object::Create_Mutex();
			endingEvent_.lock()->Set("./data/event/Ending.txt");
		}
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�w�i");
		ge->KillAll_G("�V�X�e��");
		bgm::Stop("ending");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = TitleScene::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		bgm::Play("ending");
		auto inp = ge->in1->GetState();

		if (inp.SE.down) {
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		if (endingEvent_.lock())
			Kill();

		++frameCount;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "End");

		{//���U���g
			//ML::Box2D draw(0, 0, 512, 124);
			//draw.x = (int)ge->screenCenterPos.x - 140;
			//draw.y = (int)ge->screenCenterPos.y - 150;
			//ML::Box2D src(0, 0, 256, 64);
			//this->res->resultImg->Draw(draw, src);
		}
		
		//drawScore->SetDrawValue(ge->score);

		//PressStartKey
		//if (frameCount == 60) {
		//	auto PressStartKey = FlashDraw::Object::Create(true);
		//	MyPG::MyGameEngine::DrawInfo drawInfo{
		//		DG::Image::Create("./data/image/click.png"),
		//		ML::Box2D(-143, -32, 286, 64),
		//		ML::Box2D(0, 0, 286, 64),
		//		ML::Vec2(ge->screenCenterPos.x, ge->screenHeight - 160.0f)
		//	};
		//	PressStartKey->SetUp(drawInfo, 30, 0.3f);
		//}
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