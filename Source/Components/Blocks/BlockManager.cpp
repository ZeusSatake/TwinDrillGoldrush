//-------------------------------------------------------------------
//�u���b�N�}�l�[�W���[
//-------------------------------------------------------------------
#include	"../../../MyPG.h"
#include	"BlockManager.h"
#include	"../../Scene/Task_Map.h"

								//*��	*num	*�j��
#include	"Task_Stone.h"		//��		.6		.��
#include	"Task_Bedrock .h"	//���	.7		.�s��
#include	"Task_IronOre.h"	//�S�z��	.11		.��

namespace	Manager
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	int Object::CreatBlocks(const int inp, const ML::Point pos)
	{
		auto pos_ = ML::Vec2(pos.x, pos.y);
		//�擾�ԍ�����^�X�N����
			//�j��s�u���b�N�������ꍇ���̃u���b�N�ԍ��̂܂ܕԂ�
		switch (inp)
		{
		case 2:
			if (auto b = Bedrock::Object::Create(true)) {
				b->pos = pos_;
			}
			return 2;

		case 6:
			if (auto b = Stone::Object::Create(true)) {
				b->pos = pos_;
			}
			return 0;
			break;

		case 7:
			if (auto b = Bedrock::Object::Create(true)) {
				b->pos = pos_;
			}
			return 7;	//�j��s�̂���
			break;

		case 11:
			if (auto b = IronOre::Object::Create(true)){
				b->pos = pos_;
			}
			return 0;
			break;

		default:
			return 0;
			break;
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