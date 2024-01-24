//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../MyPG.h"

#include  "Task_EventEngine.h"
#include "Task_Ev_Message.h"
#include "Task_Ev_Input.h"
#include "Task_Ev_Image.h"
#include "Task_Ev_FadeInOut.h"

#include "../Actors/Task_Player.h"
#include "../Actors/Task_Drill.h"
#include "../Scene/GameScene.h"


namespace  EventEngine
{
	Object::WP Object::instance;
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

		//�C�x���g���s���ɓ����ƍ�����̂͂��ׂĒ�~������
		ge->StopAll_GN(player::defGroupName, player::defName);
		ge->StopAll_GN(drill::defGroupName, drill::defName);
		ge->StopAll_G("�G");
		ge->StopAll_G(GameScene::defGroupName);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//�C�x���g���s���ɒ�~�����Ă����^�X�N�����s��Ԃɖ߂�
		ge->StopAll_GN(player::defGroupName, player::defName, false);
		ge->StopAll_GN(drill::defGroupName, drill::defName, false);
		ge->StopAll_G("�G", false);
		ge->StopAll_G(GameScene::defGroupName, false);
		
		//���f�[�^���^�X�N���
		if (this->evFile.is_open())
		{
			this->evFile.close();//�t�@�C�������
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (!this->evFile)
		{
			this->Kill();//�t�@�C�����Ȃ��Ȃ����
		}
		//�V�����s��ǂݍ���
		string lineText;
		string headerText;
		string dataText;
		while (this->ReadLine(lineText))
		{
			//�s�����
			string::size_type t = lineText.find(">");
			headerText = lineText.substr(0, t);
			dataText = lineText.substr(t + 1);
			//�w�b�_���ɑΉ������C�x���g�������s��
			bool rtv = this->Execute(headerText, dataText);
			//�C�x���g�������s�������͎��g�̏�Ԃ��A�N�e�B�u�łȂ��Ȃ��Ă����ꍇ���[�v�𔲂���
			if (false == rtv ||
				BTask::State::Active != this->CheckState())
			{
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//�C�x���g���s�G���W���𐶐����܂�
	//���ɃC�x���g���s�G���W�������݂���ꍇ�A�����͎��s���܂�
	Object::SP Object::Create_Mutex()
	{
		//�V�K�쐬���X�V���̔���
		if (auto p = instance.lock())
		{
			return nullptr;//���ɑ��݂���
		}
		else//�V�K�쐬
		{
			p = Object::Create(true);
			instance = p;
			return p;
		}
	}

	//-------------------------------------------------------------------
	//�C�x���g�t�@�C������o�^���C�x���g�J�n
	bool Object::Set(const string& fPath_)
	{
		//���������̃t�@�C�������
		if (this->evFile.is_open())
		{
			this->evFile.close();
		}
		//�C�x���g�t�@�C���ǂݍ���
		this->evFile.open(fPath_);
		//�t�@�C�����J���Ȃ��i���s
		if (!this->evFile)
		{
			return false;
		}
		return true;
	}

	//-------------------------------------------------------------------
	//��s���̃f�[�^��ǂݍ���
	bool Object::ReadLine(string& lineT_)
	{
		//��s�ǂݍ���
		bool rtv= false;
		while (getline(this->evFile, lineT_))
		{
			//�������`�F�b�N
			//�s���ɋ󔒁i�S�p�󔒂��j�ȊO�̕������Ȃ�
			if (string::npos == lineT_.find_first_not_of(" �@"))
			{
				continue;
			}
			//�ŏ��̕�����/�̏ꍇ�R�����g�Ƃ݂Ȃ�
			if ('/' == lineT_.at(0))
			{
				continue;
			}
			//�w�b�_���ƃf�[�^�̋��ڂ����遄���܂܂�Ă��Ȃ�
			if (string::npos == lineT_.find(">"))
			{
				continue;
			}
			//�������`�F�b�N�I��
			rtv = true;
			break;
		}
		return rtv;
	}

	//-------------------------------------------------------------------
	//�s�f�[�^�ɑ΂��鏈�����s��
	bool Object::Execute(string& hs_, string& ds_)
	{
		//�f���~�^�i��؂�L���j�폜�i�ׂ������@�`�F�b�N�͂��Ă��Ȃ��j
		string::size_type t;
		while ((t = ds_.find_first_of("(,);")) != string::npos)
		{
			ds_[t] = ' ';
		}
		//
		stringstream ss;
		ss << ds_;
		//�C�x���g�I��
		if (hs_ == "end")
		{
			this->Kill();
		}
		//���b�Z�[�W�\��
		else if (hs_ == "msg")
		{
			Ev_Message::Object::CreateOrReset(ss);
		}
		//�摜�ǂݍ��݁��\��
		else if (hs_ == "evimg")
		{
			Ev_Image::Object::CreateOrReset(ss);
		}
		//����
		else if (hs_ == "input")
		{
			Ev_Input::Object::CreateOrReset(ss);
		}
		//��������
		else if (hs_ == "if")
		{
			this->If(ss);
		}
		//�C�x���g�t���O�f�[�^�ύX
		else if (hs_ == "flag")
		{
			this->EventFlag(ss);
		}
		else if (hs_ == "fade_io")
		{
			Ev_FadeInOut::Object::CreateOrFadeIn(ss);
		}
		else if (hs_ == "img")
		{
			this->Image(ss);
		}
		else if (hs_ == "fileset")
		{
			this->FileSet(ss);
		}
		//���x��
		else if (hs_ == "label")
		{

		}
		/*else if (hs_ == "appear")
		{
			this->AppearObject(ss);
		}*/
		else if (hs_=="map")
		{
			this->MapLoad(ss);
		}
		/*else if (hs_ == "chara")
		{
			this->ModifyChara(ss);
		}*/
		else if (hs_=="kill")
		{
			this->KillObject(ss);
		}
		else
		{
			return false;
		}
		return true;
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