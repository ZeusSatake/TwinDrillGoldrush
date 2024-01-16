//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../MyPG.h"
#include  "Task_Ev_Message.h"
#include "../../Character.h"
#include "../Actors/Task_Player.h"
#include "../System/Task_Save.h"
#include  "../Actors/Task_MiningResult.h"

namespace Ev_Message
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img[0] = DG::Image::Create("./data/image/MessageF.png");
		this->img[1] = DG::Image::Create("./data/image/message.png");//�ȉ���
		this->img[2] = DG::Image::Create("./data/image/MessageF.png");
		this->img[3] = DG::Image::Create("./data/image/MessageF.png");
		this->img[4] = DG::Image::Create("./data/image/MessageF.png");
		this->img[5] = DG::Image::Create("./data/image/MessageF.png");
		this->img[6] = DG::Image::Create("./data/image/MessageF.png");
		this->img[7] = DG::Image::Create("./data/image/MessageF.png");
		this->img[8] = DG::Image::Create("./data/image/MessageF.png");
		this->img[9] = DG::Image::Create("./data/image/MessageF.png");

		this->font[0] = DG::Font::Create("HG�ۃS�V�b�NM-PRO", 8, 16);
		this->font[1] = DG::Font::Create("HG�ۃS�V�b�NM-PRO", 12, 24);
		this->font[2] = DG::Font::Create("HG�ۃS�V�b�NM-PRO", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < _countof(this->img); ++i)
		{
			this->img[i].reset();
		}
		for (int i = 0; i < _countof(this->font); ++i)
		{
			this->font[i].reset();
		}
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
		this->Stop();
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = 0;
		this->pos.y = ge->screen2DHeight - 128;//270
		this->msgText = "";
		this->bgNumber = 1;
		this->fontNumber = 1;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		//�������Ԍo�߂������̓{�^���������ꂽ�Ƃ�
		if ((this->timeLimit != 0 && this->timeCnt >= this->timeLimit) || inp.B2.down)
		{
			//���g�͒�~��Ԃɂ���
			this->Stop();
			//�G���W���̒�~��Ԃ���������
			ge->StopAll_GN("�C�x���g", "���s�G���W��", false);
		}
		else
		{
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screen2DWidth, 128);
		ML::Box2D src(0, 0, 1280, 206);
		draw.Offset(this->pos);
		this->res->img[this->bgNumber]->Draw(draw, src, ML::Color(1, 1, 1, 1));
		ML::Box2D drawF(50, 15, ge->screen2DWidth - 12, 128 - 12);
		drawF.Offset(this->pos);
		this->res->font[this->fontNumber]->Draw(drawF, this->msgText);
	}

	//-------------------------------------------------------------------
	void  Object::Set(const string& taskName_, stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		int timeL;
		string msg;
		ss_ >> timeL >> msg;
		//�^�X�N����ݒ�
		this->name = taskName_;
		//���b�Z�[�W�g�����v���֑Ή�����
		if (msg == "off")
		{
			this->Kill();
			return;
		}
		this->timeCnt = 0;
		this->timeLimit = timeL;
		this->msgText = msg;
		//*�����s�ɒu��������
		string::size_type ast;
		while ((ast = msgText.find("*")) != string::npos)
		{
			msgText.at(ast) = '\n';
		}

		//&�𖼑O�ɒu��������
		string::size_type ast1;

		while ((ast1 = msgText.find("&")) != string::npos)
		{
			msgText.replace(ast1, 1, ge->playerPtr->GetPersonalName());
		}

		//%��ڕW�z�΂ɒu��������
		string::size_type ast2;

		while ((ast2 = msgText.find("%")) != string::npos)
		{
			string orename[5] = { "�_�}�X�J�X","�I���n���R��","�q�q�C���J�l","�A�_�}���^�C�g","���f�B�A���^�C�g" };
			auto save = Save::Object::Create(true);
			msgText.replace(ast2, 1, orename[save->GetValue<int>(Save::Object::ValueKind::StageNo)]);
		}

		//�C�x���g�G���W�����ꎞ��~������
		ge->StopAll_GN("�C�x���g", "���s�G���W��");
		//���g�̒�~��Ԃ���������
		this->Stop(false);
	}

	//-------------------------------------------------------------------
	void Object::CreateOrReset(stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		string taskName;
		ss_ >> taskName;

		//�V�K�쐬���X�V�̔��f
		auto p = ge->GetTask<Object>("���b�Z�[�W�\���g", taskName);
		//�V�K�쐬�̏ꍇ
		if (nullptr == p)
		{
			p = Object::Create(true);
			p->Set(taskName, ss_);
		}
		//�X�V�̏ꍇ
		else
		{
			p->Set(taskName, ss_);
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