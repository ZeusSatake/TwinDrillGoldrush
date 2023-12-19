#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�C�x���g�������̉摜�\��
//-------------------------------------------------------------------
#include "../../MyPG.h"//
#include "../../GameObject.h"

namespace  Ev_Image
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�C�x���g�摜");	//�O���[�v��
	const  string  defName(				"NoName");	//�^�X�N��
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
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		DG::Image::SP img;
		ML::Vec2 pos;
		ML::Box2D drawBase;
		ML::Box2D src;

		//�^�X�N�𐶐����邩�A�����̃^�X�N�̏����X�V����
		static void CreateOrReset(stringstream& ss_);
		//�\�����郁�b�Z�[�W��ݒ肷��
		void Set(const string& taskName_, stringstream& ss_);
	};
}