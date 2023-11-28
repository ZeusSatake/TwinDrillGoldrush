#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../Actor.h"

namespace  EnemyMap
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{��");	//�O���[�v��
	const  string  defName("�G�}�b�v");	//�^�X�N��
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
	class  Object : public  Actor
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
	private:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		DG::Image::SP img;
		int  arr[160][160];//yx
		int					sizeY, sizeX;
		int chipSize;
		ML::Box2D			hitBase;//�s�N�Z���P�ʂ̃}�b�v�T�C�Y������
		ML::Box2D  chip[32];
	public:
		bool  Load(const  string& fileName);//�}�b�v�ǂݍ���
		void SetEnemy();
	};
}
		