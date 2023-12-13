#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�C�x���g���s�G���W��
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"
#include "../../MyPG.h"
#include "../../GameObject.h"

namespace  EventEngine
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�C�x���g");	//�O���[�v��
	const  string  defName(				"���s�G���W��");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		
		Resource::SP	res;
	private:
		Object();
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		static WP instance;
		ifstream evFile;

		//�C�x���g���s�G���W���𐶐����܂��B
		//���ɃC�x���g���s�G���W�������݂���ꍇ�A�����͎��s���܂�
		static Object::SP Create_Mutex();
		//�C�x���g�t�@�C������o�^���C�x���g�J�n
		bool Set(const string& fPath_);
		//��s���̃f�[�^��ǂݍ���
		bool ReadLine(string& lineT_);
		//�s�f�[�^�ɑ΂��鏈�����s��
		bool Execute(string& hs_, string& ds_);

		//�V�X�e������n
		//�C�x���g�t���O�̃f�[�^�̏�������
		bool EventFlag(stringstream& ss_);
		//��������
		bool If(stringstream& ss_);
		bool Image(stringstream& ss_);
		bool FileSet(stringstream& ss_);
		//bool AppearObject(stringstream& ss_);
		bool MapLoad(stringstream& ss_);
		//bool ModifyChara(stringstream& ss_);
		bool KillObject(stringstream& ss_);
	};
}