//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "../../randomLib.h"
#include  "../../Task_Effect00.h"
#include  "../../sound.h"
#include  "../../easing.h"

#include  "Task_Title.h"
#include  "Task_Game.h"

#include  "../System/Task_BackGround.h"
#include  "../System/Task_FlashDraw.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		TitleLogo01 = DG::Image::Create("./data/image/title.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		TitleLogo01.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;

		//�f�o�b�O�p�t�H���g�̏���
		this->TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���y�t�@�C���̓ǂݍ���
		//BGM
		// BGM�̗��p����t�@�C���͋Ȃ̒����ɂ���ăT�C�Y���ς��܂��B
		// ��ʓI��Wav�͒��g���g�`�f�[�^�ƌĂ΂��T�C�Y���傫�ȃf�[�^�ɂ�܂��B
		// ���mp3�̓l�b�g�Ȃǂł̈�����z�肵�����k�`���ł��B
		// BGM��p����ꍇ��mp3�`���̃t�@�C���𗘗p���܂��傤�B
		// ���Ȃ݂ɂ��̃T���v���̃t�@�C����
		// �Đ�����1�F30�قڂ̋Ȃ�
		// mp3 = 4.3MB wav = 19MB�ł��B
		//bgm::LoadFile("bgm1", "./data/sound/TitleBGM.mp3");
		//bgm::Play("bgm1");

		//se
		// se�͌��ʉ��ł��B
		// bgm�Ƃ̍ő�̈Ⴂ�͂ЂƂ̉��y�t�@�C���̓����Đ��ɑΉ����Ă��邱�Ƃł��B
		// se��wav�t�@�C�������������Ƃ��o���܂���Bmp3�̓G���[�ɂȂ�܂��B
		// �����Đ��̕K�v���Ȃ����̂�BGM
		// �����Đ����K�v�Ȃ��̂�se�Ǝg�������Ă��������B
	  // �܂����̃T���v���̂悤�ȓ��{��t�@�C�����͂�߂������ǂ��ł��B
		// �ǂݍ��݃G���[�̌��ɂȂ�܂��B
		//se::LoadFile("se1","./data/sound/se/�u�������������v.wav");

//		se::LoadFile("se2", "./data/sound/se/�u�撣�����ˁv.mp3");����̓G���[ wav����Ȃ��ƃ_��
		//se::LoadFile("se2", "./data/sound/se/�u�撣�����ˁv.wav");

		//se::LoadFile("ok", "./data/sound/ok.wav");

		easing::Set("titleX", easing::CIRCOUT, -200, (float)ge->screenCenterPos.x, 120);
		easing::Start("titleX");
		easing::Set("titleY", easing::BOUNCEOUT, -200, (float)ge->screenCenterPos.y, 120);
		easing::Start("titleY");

		frameCnt = 0;

		//���^�X�N�̐���
		auto backGound = BackGround::Object::Create(true);
		backGound->SetUp(
			"./data/image/titleback.png",
			ML::Point{ 1920, 1200 },
			ML::Point{ (int)ge->screenWidth, (int)ge->screenHeight });
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�w�i");
		//bgm::Stop("bgm1");
		ge->KillAll_G("�V�X�e��");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		++frameCnt;
		auto inp = ge->in1->GetState();

		if (inp.SE.down) {
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(98, ML::Vec2(0, 0));
			//se::Play("ok");
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		easing::UpDate();
		this->logoY = easing::GetPos("titleY");
		this->logoX = easing::GetPos("titleX");

		if (frameCnt == 150) {
			auto PressStartKey = FlashDraw::Object::Create(true);
			MyPG::MyGameEngine::DrawInfo drawInfo{
				DG::Image::Create("./data/image/click.png"),
				ML::Box2D(-143, -32, 286, 64),
				ML::Box2D(0, 0, 286, 64),
				ML::Vec2(ge->screenCenterPos.x, ge->screenHeight - 160.0f)
			};
			PressStartKey->SetUp(drawInfo, 30, 0.3f);
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 792, 89);
		ML::Box2D draw((int)(-src.w * 0.5f), 0, src.w, src.h);
		draw.Offset(ML::Vec2(logoX, logoY));
		res->TitleLogo01->Draw(draw, src);
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